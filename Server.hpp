//
// Created by sfcdo on 20.05.2021.
//

#ifndef SERVER_HPP_
#define SERVER_HPP_
#include "allowed_library_includes.hpp"
#include "ServerConfig.hpp"
#include "Request.hpp"

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
  unsigned max_fd;
  unsigned status;
  void Init();
  Server();
  int Guard(int retval, bool rw_operation);
  void ConnectionAccept();
  void SocketRead();
  void SocketWrite();
  const char * Response(std::string& request);
  bool valid_method(size_t begin, size_t end, std::string & request_line);
};
#endif // SERVER_HPP_
