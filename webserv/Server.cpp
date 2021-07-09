//
// Created by sfcdo on 20.05.2021.
//

#include "Server.hpp"
#include "Response.hpp"

char webpage[] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type text/html; charset=UTF-8\r\n\r\n"
    "<!DOCTYPE html>\r\n"
    "<html><head><title>webserv</title>\r\n"
    "<style>body {background-color: #FFFF00}</style></head>\r\n"
    "<body><center><h1>Hello world!</h1><br>\r\n";




Server::Server(const std::vector<ServerConfig> &config, const ssize_t INPUT_BUFFER_SIZE)
    : config(config), master_read(), working_read(), master_write(), working_write(),
    timout(), INPUT_BUFFER_SIZE(INPUT_BUFFER_SIZE), status(), max_fd(-1) {}

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
        for (read_iterator it = read.begin(); it != read.end(); it++)
          close(it->fd);
        for (write_iterator it = write.begin(); it != write.end(); it++)
          close(it->fd);
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
//      std::cout << "No incoming connections last 3 seconds" << std::endl;
    } else {
//      std::cout << "Server ready for connections" << std::endl;
      ConnectionAccept();
      SocketRead();
      SocketWrite();
    }
  }
}
void Server::ConnectionAccept() {
  int client_fd;
  for (server_iterator it = server.begin(); it != server.end(); it++) {
    if (FD_ISSET(it->fd, &working_read)) {
//      std::cout << "Listening socket " << it->first << " is ready for incoming connections" << std::endl;
      while ((client_fd = Guard(accept(it->fd, NULL, NULL), false)) != -1) {
//        std::cout << "New connection accepted with fd = " << client_fd << std::endl;
        fcntl(client_fd, F_SETFL, O_NONBLOCK);
        FD_SET(client_fd, &master_read);
        read.push_back(ReadElement(client_fd));
      }
//      std::cout << "Ended handle of incoming connections with max_fd = " << max_fd << std::endl;

    }
  }
}
void Server::SocketRead() {
  char *buf = reinterpret_cast<char *>(calloc(INPUT_BUFFER_SIZE, sizeof(char)));
  for (read_iterator it = read.begin(); it != read.end(); it++) {
    if (FD_ISSET(it->fd, &working_read)) {
      for (int i = 0; recv(it->fd, buf, INPUT_BUFFER_SIZE, 0) != -1; i++) {
        ProcessInputBuffer(buf, it->request);
        memset(buf, 0, INPUT_BUFFER_SIZE);
      }
    }
    if (it->request.formed) {
      it->request.buffer.clear();
      FD_CLR(it->fd, &master_read);
      FD_SET(it->fd, &master_write);
      write.push_back(WriteElement(it->fd, it->request));
      read.erase(it--);
    }
  }
}

void Server::ProcessInputBuffer(char *buffer, Request &request) {
  request.buffer += buffer;
  if (!request.headersReady)
    GetHeaders(request);
  if (request.headersReady)
    GetBody(request);
}

void Server::GetHeaders(Request & request) {
  size_t pos;
  if ((pos = request.buffer.find("\r\n\r\n", 0)) == std::string::npos)
    return;
  if (validator_.ValidHeaders(request.buffer))
    parser_.ProcessHeaders(request);
  else
    request.SetFailed(validator_.GetStatusCode());
  request.headersReady = true;
  request.AdjustHeaders();
  request.buffer = request.buffer.substr(pos + 4);
}

void Server::GetBody(Request &request) {
  if (request.chunked) {
    if ((request.buffer.find("\r\n\r\n", 0)) == std::string::npos)
      return;
    if (validator_.ValidBody(request.buffer))
      parser_.ParseBody(request);
    else
      request.SetFailed(validator_.GetStatusCode());
  } else {
    if (request.buffer.length() >= request.content_length) {
      request.body = request.buffer.substr(0, request.content_length);
      request.formed = true;
    }
  }
}

void Server::SocketWrite() {
  for (write_iterator it = write.begin(); it != write.end(); it++) {
    if (FD_ISSET(it->fd, &working_write)) {
      if ((status = Guard(send(it->fd, SendResponse(it->request), kek, 0), true)) != -1)
//        std::cout << status << " bytes answered to client with socket fd = " << it->first << std::endl;
      close(it->fd);
      FD_CLR(it->fd, &master_write);
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
    server.push_back(ServerElement(server_fd, addr));
    max_fd = server_fd;
  }
}



const char * Server::SendResponse(Request & req) {
  if (!req.failed) {
    req.PrintRequestLine();
    req.PrintHeaders();
    req.PrintBody();
    std::cout << std::endl << std::endl;
  }
  else
    std::cout << "Request sucks" << std::endl;
  Response resp;
  resp.freeResponse();
  std::string hui =  resp.SetResponseLine(req.GetRequestLine(), config.front());
  kek = hui.length();
  return hui.c_str();
}

