//
// Created by sfcdo on 20.05.2021.
//

#ifndef WEBSERV_SERVER_HPP_
#define WEBSERV_SERVER_HPP_
#include "allowed_library_includes.hpp"
#include "ServerConfig.hpp"
#include "Request.hpp"
#include "MessageParser.hpp"
#include "MessageValidator.hpp"

class Server {
 public:
  Server(const std::vector<ServerConfig>& config, ssize_t INPUT_BUFFER_SIZE);
  void Run();
  ~Server();
 private:
  const std::vector<ServerConfig> config;
  std::vector<std::pair<int, sockaddr_in> > server;
  std::vector<int> read;
  std::vector<std::pair<int, std::string> > write;
  std::vector<sockaddr_in> server_addr;
  fd_set master_read;
  fd_set working_read;
  fd_set master_write;
  fd_set working_write;
  timeval timout;
  const ssize_t INPUT_BUFFER_SIZE;
  int max_fd;
  unsigned status;
  MessageValidator validator_;
  MessageParser parser_;
  Request request_;
  void Init();
  Server();
  int Guard(ssize_t retval, bool rw_operation);
  void ConnectionAccept();
  void SocketRead();
  void SocketWrite();
  const char * Response(std::string& request);
};
#endif // WEBSERV_SERVER_HPP_
