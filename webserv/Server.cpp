//
// Created by sfcdo on 20.05.2021.
//

#include "Server.hpp"
#include "Response.hpp"
# include <sys/time.h>
#include <sys/wait.h>




Server::Server(const std::vector<ServerConfig> &config, const ssize_t INPUT_BUFFER_SIZE)
    : config(config), master_read(), working_read(), master_write(), working_write(),
      timeout(), INPUT_BUFFER_SIZE(INPUT_BUFFER_SIZE), status(), isHeader(true) {}

Server::Server() : master_read(), working_read(), master_write(), working_write(),
                   timeout(), INPUT_BUFFER_SIZE(DEFAULT_INPUT_BUFFERSIZE), status(), isHeader(true) {}

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
        std::cout << "Recv/Send error occured with status = -1" << std::endl;
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
    timeout.tv_sec = TIMOUT_SEC;
    if ((status = Guard(select(FD_SETSIZE, &working_read, &working_write, NULL, &timeout), false)) == 0) {
//      std::cout << "No incoming connections last " << timeout.tv_sec << " seconds" << std::endl;
    } else {
//      std::cout << "Server ready for connections" << std::endl;
      ConnectionAccept();
//      std::cout << std::setw(50) << "END OF ACCEPT CYCLE | read vector size = " << read.size() << " write vector size = " << write.size() << std::endl;
      SocketRead();
//      std::cout << std::setw(50) << "END OF READ CYCLE | read vector size = " << read.size() << " write vector size = " << write.size() << std::endl;
      SocketWrite();
//      std::cout << std::setw(50) << "END OF WRITE CYCLE | read vector size = " << read.size() << " write vector size = " << write.size() << std::endl;
    }
  }
}
void Server::ConnectionAccept() {
  int client_fd;
  for (server_iterator it = server.begin(); it != server.end(); it++) {
    if (FD_ISSET(it->server_fd, &working_read)) {
//      std::cout << "Listening socket " << it->server_fd << " is ready for incoming connections" << std::endl;
      if ((client_fd = Guard(accept(it->server_fd, NULL, NULL), false)) != -1) {
//        PrintLog(it, "accepted client connection", client_fd);
        fcntl(client_fd, F_SETFL, O_NONBLOCK);
        FD_SET(client_fd, &master_read);
        read.push_back(ReadElement(it->server_fd, client_fd, it->server_config, GetTimeInSeconds()));
      }
//      std::cout << "Ended handle of incoming connections with max_fd = " << max_fd << std::endl;

    }
  }
}

std::string update_string_to_see_crlf(std::string in) {
  size_t pos;
  while ((pos = in.find('\r')) != -1)
    in.replace(pos, 1, "\\r");
  while ((pos = in.find('\n')) != -1)
    in.replace(pos, 1, "\\n");
  return in;
}


void Server::SocketRead() {
  for (read_iterator it = read.begin(); it != read.end(); it++) {
    if (FD_ISSET(it->fd, &working_read)) {
      PrintLog(it, "client IS_SET for read", it->fd);
      for (; (status = recv(it->fd, buf, INPUT_BUFFER_SIZE, 0)) != -1 && status;) {
        std::stringstream ss;
//        ss << "recv value = " << status  << "|buf = [" << buf << "]";
//        PrintLog(it, ss.str(), it->fd);
        it->last_read = GetTimeInSeconds();
//        if (it->request.request_line["method"] == "POST")
//          std::cout << "buf:"<<buf << "$" << std::endl;
        ProcessInputBuffer(buf, it->request);
        memset(buf, 0, INPUT_BUFFER_SIZE);
//        it->request.PrintRequestLine();
//        std::cout << "Source request was:" << std::setw(90) << it->request.source_request << std::endl;
      }
    }
    it->last_action_time = (GetTimeInSeconds() - it->last_read);
//    std::cout << "status = " << status << std::endl;
    if (!status || it->last_action_time > 10) {
      ClearBrokenConnection(it->fd);
      read.erase(it--);
    } else if (it->request.formed) {
      it->request.Print();
      FD_CLR(it->fd, &master_read);
      FD_SET(it->fd, &master_write);
      write.push_back(WriteElement(it->server_fd, it->fd, it->request));

      if (!it->request.keep_alive) {
//        PrintLog(it, "ended read by not keep alive behavior", it->fd);
//        std::cout << "Client_fd = " << it->fd << " read ended due not keep alive connection" << std::endl;
        read.erase(it--);
      }
      it->request = Request(it->request.buffer, it->request.server_config);
    }
  }
}

void Server::ProcessInputBuffer(char *buffer, Request &request) {
  request.buffer += buffer;
  request.source_request += buffer;
  size_t pos;
  if (!request.recieved_headers) {
    if((pos = request.buffer.find("\r\n\r\n", 0)) == std::string::npos)
      return;
    GetHeaders(request);
    request.buffer = request.buffer.substr(pos + 4);
  }
  if (!request.recieved_body) {
    std::cout << "size of buf = " << request.buffer.length() << std::endl;
    if (request.chunked) {
      if ((pos = request.buffer.find("0\r\n\r\n")) == std::string::npos)
        return;
      GetChunkedBody(request);
    }
    else {
      if (request.buffer.length() < request.content_length)
        return;
      GetBody(request);
    }
  }
}

void Server::GetHeaders(Request & request) {
  if (validator_.ValidHeaders(request.buffer))
    parser_.ProcessHeaders(request);
  else
    request.SetFailed(validator_.GetStatusCode());
  request.recieved_headers = true;
  request.AdjustHeaders();
}

void Server::GetBody(Request &request) {
  request.body = request.buffer.substr(0, request.content_length);
  request.recieved_body = true;
  request.buffer.clear();
}

void Server::GetChunkedBody(Request &request) {
  std::cout << "buffer before body parsing length = " << request.buffer.length() << std::endl;
  request.formed = true;
  if (validator_.ValidBody(request.buffer))
    parser_.ParseBody(request);
  else {
    std::cout << "VALIDATOR ENCOUNTERED INVALID CHUNKED BODY. REQUEST FAILED" << std::endl;
    request.SetFailed(validator_.GetStatusCode());
  }
  if (request.failed)
    std::cout << "FAILED !!!!!!! " << std::endl;
  std::stringstream ss;
    ss << request.body.length();
  request.headers.insert(std::make_pair("Content-Length", ss.str()));
  request.recieved_body = true;
  request.buffer.clear();
}

void Server::SocketWrite() {
  for (write_iterator it = write.begin(); it != write.end(); it++) {
    if (!it->request.formed)
      return;
    if (FD_ISSET(it->fd, &working_write)) {
      if ((status = Guard(
          send(it->fd, &it->output.c_str()[it->send_out_bytes], it->out_length - it->send_out_bytes, 0),
          true
      )) != -1) {
        std::cout << "sending response with size = " << it->out_length << ":" << std::endl;
//        PrintLog(it, it->output, it->fd);
//        std::cout << std::endl;
        it->send_out_bytes += status;
        std::stringstream ss;
        if (!it->out_length) {
          ss << "[ WARNING! Response length = 0 to request with length = " <<
             it->request.source_request.length() << "] ";
        }
        ss << status << "/" << it->out_length << " bytes send";
        PrintLog(it, ss.str(), it->fd);
        if (it->send_out_bytes == it->out_length) {
          FD_CLR(it->fd, &master_write);
          if (!it->request.keep_alive) {
//            PrintLog(it, "closed connection (disabled keep-alive)", it->fd);
//            std::cout << "closed client_fd = " << it->fd << " connection due not keep alive" << std::endl;
            close(it->fd);
          }
          else
              FD_SET(it->fd, &master_read);
          write.erase(it--);
        }
      }
//      if (status == -1)
//        PrintLog(it, "send returned -1 trying to x", it->fd);
    }
  }
}

Server::~Server() {
  free(buf);
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
    server.push_back(ServerElement(server_fd, addr, *it));
  }
  buf = reinterpret_cast<char *>(calloc(INPUT_BUFFER_SIZE, sizeof(char)));
}

template<class Iterator>
void Server::PrintLog(Iterator it, const std::string & msg, int client_fd) {
  std::cout << "Server #" << it->server_fd << " " <<
            std::setw(90) <<  msg << std::setw(10) << "| Client#" << client_fd << std::endl;
}

long Server::GetTimeInSeconds() {
  gettimeofday(&timev, NULL);
  return timev.tv_sec;
}

void Server::ClearBrokenConnection(int fd) {
  for(write_iterator wit = write.begin(); wit != write.end(); wit++)
    if (wit->fd == fd) {
      FD_CLR(wit->fd, &master_write);
      write.erase(wit--);
    }
  FD_CLR(fd, &master_read);
  close(fd);
}
