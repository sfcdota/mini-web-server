#include "Server.hpp"

template <class T>
bool Compare(const std::pair<int, T> p1, const std::pair<int, T> p2) {
  return p1.first < p2.first;
}


char webpage[] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Length 154\r\n"
    "Content-Type text/html; charset=UTF-8\r\n\r\n"
    "<!DOCTYPE html>\r\n"
    "<html><head><title>webserv</title>\r\n"
    "<style>body {background-color: #FFFF00}</style></head>\r\n"
    "<body><center><h1>Bla bla!</h1><br>\r\n";




Server::Server(const std::vector<ServerConfig> &config, const ssize_t INPUT_BUFFER_SIZE)
    : config(config), master_read(), working_read(), master_write(), working_write(),
    timout(), INPUT_BUFFER_SIZE(INPUT_BUFFER_SIZE), status(), max_fd(-1), request_() {}

Server::Server() : master_read(), working_read(), master_write(), working_write(),
timout(), INPUT_BUFFER_SIZE(DEFAULT_INPUT_BUFFERSIZE), max_fd(), status() {}

/// Guard for libc function errors
/// \param retval returned function value, that will go through this function if success.
/// \param rw_operation enables check for errno only when false (not read-write operation).
/// \return inputed retval if not exited
int Server::Guard(ssize_t retval, bool rw_operation) {
  if (retval == -1) {
    if (!rw_operation) {
      if (EWOULDBLOCK != errno && EAGAIN != errno) {
        std::cout << strerror(errno) << std::endl;
        for (std::vector<int>::iterator it = read.begin(); it != read.end(); it++)
          close(*it);
        for (std::vector<std::pair<int, std::string> >::iterator it = write.begin(); it != write.end(); it++)
          close(it->first);
        exit(EXIT_FAILURE);
      }
    } else {
        std::cout << "Read/write error occured" << std::endl;
        return -1;
      }
  }
  return retval;
}

void Server::Run() {
  Init();
  while (true) {
    memcpy(&working_read, &master_read, sizeof(master_read));
    memcpy(&working_write, &master_write, sizeof(master_write));
    timout.tv_sec = TIMOUT_USEC;
    if ((status = Guard(select(FD_SETSIZE, &working_read, &working_write, NULL, &timout), false)) == 0) {
      std::cout << "No incoming connections last 3 seconds" << std::endl;
    } else {
      std::cout << "Server ready for connections" << std::endl;
      ConnectionAccept();
      SocketRead();
      SocketWrite();
    }
  }
}
void Server::ConnectionAccept() {
  int client_fd;
  for (std::vector<std::pair<int, sockaddr_in> >::iterator it = server.begin(); it != server.end(); it++) {
    if (FD_ISSET(it->first, &working_read)) {
      std::cout << "Listening socket " << it->first << " is ready for incoming connections" << std::endl;
      while ((client_fd = Guard(accept(it->first, NULL, NULL), false)) != -1) {
        std::cout << "New connection accepted with fd = " << client_fd << std::endl;
        fcntl(client_fd, F_SETFL, O_NONBLOCK);
        FD_SET(client_fd, &master_read);
        read.push_back(client_fd);
      }
      std::cout << "Ended handle of incoming connections with max_fd = " << max_fd << std::endl;
    }
  }
}
void Server::SocketRead() {
  std::string input;
  bool empty_line = false;
  char *buf = reinterpret_cast<char *>(calloc(INPUT_BUFFER_SIZE, sizeof(char)));
  for (std::vector<int>::iterator it = read.begin(); it != read.end(); it++) {
    if (FD_ISSET(*it, &working_read)) {
      std::cout << "Trying to read from client with socked fd = " << *it << std::endl;
      while (!empty_line && (status = (Guard(recv(*it, buf, INPUT_BUFFER_SIZE, 0), true))) > 0) {
        std::cout << status << " bytes received from client with socket fd = " << *it << std::endl;
        input += std::string(buf);
        if ((empty_line = !strncmp("\r\n\r\n", &buf[status - 4], 4)))
          std::cout << "Client empty line found! Server will close connection" << std::endl;
        memset(buf, 0, INPUT_BUFFER_SIZE);
      }
//      std::cout << "input string :" << std::endl << input << "%%%%%%%%%%%%%%%%%%%%%%%" <<std::endl;
      FD_CLR(*it, &master_read);
      FD_SET(*it, &master_write);
      write.push_back(std::pair<int, std::string>(*it, input));
      read.erase(it--);
    }
  }
}

void Server::SocketWrite() {
  for (std::vector<std::pair<int, std::string> >::iterator it = write.begin(); it != write.end(); it++) {
    if (FD_ISSET(it->first, &working_write)) {
    	const char *bla = ResponsePrep(it->second).c_str();
		printf("%s", bla);
      if ((status = Guard(send(it->first, bla, strlen(webpage), 0), true)) != -1)
        std::cout << status << " bytes answered to client with socket fd = " << it->first << std::endl;
      close(it->first);
      FD_CLR(it->first, &master_write);
      write.erase(it--);
    }
  }
}

Server::~Server() {

}

void Server::Init() {
  int options_value = 1;
  FD_ZERO(&master_read);
  FD_ZERO(&master_write);
  FD_ZERO(&working_read);
  FD_ZERO(&working_write);
  for (std::vector<ServerConfig>::const_iterator it = config.begin(); it != config.end(); it++) {
    sockaddr_in addr = {};
    addr.sin_addr.s_addr = INADDR_ANY; // should be it->host
    addr.sin_family = AF_INET;
    addr.sin_port = htons(it->port);
    int server_fd = Guard(socket(AF_INET, SOCK_STREAM, 0), false);
    // SOCK_NONBLOCK on ubuntu only!

    fcntl(server_fd, F_SETFL, O_NONBLOCK);
    Guard(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &options_value, sizeof(int)), false);
    // SO_REUSEPORT on ubuntu only!

    Guard(bind(server_fd, (struct sockaddr *) &addr, sizeof(sockaddr_in)), false);
    Guard(listen(server_fd, MAX_CONNECTIONS), false);
    FD_SET(server_fd, &master_read);
    std::pair<int, sockaddr_in> pair = std::pair<int, sockaddr_in>(server_fd, addr);
    server.push_back(pair);
    max_fd = server_fd;
  }
}

std::string Server::ResponsePrep(std::string & request) {
	conf s;
	std::string str;
	Response response_;
	s = parsConf();
  if (validator_.ValidRequest(request)) {
    request_ = parser_.ProcessRequest(request);
    request_.PrintRequestLine();
    request_.PrintHeaders();
    request_.PrintBody();
    response_.freeResponse();
    str = response_.SetResponseLine(request_.GetRequestLine(), s);
    if (str.size()) {
		return str;
	}
  }
  else
    std::cout << "Request sucks" << std::endl;
  return str;
}

