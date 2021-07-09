//
// Created by sfcdo on 20.05.2021.
//

#ifndef WEBSERV_SERVER_HPP_
#define WEBSERV_SERVER_HPP_
#include "allowed_library_includes.hpp"
#include "parser.hpp"
#include "Request.hpp"
#include "MessageParser.hpp"
#include "MessageValidator.hpp"
class Server {
 public:
  Server(const std::vector<ServerConfig>& config, ssize_t INPUT_BUFFER_SIZE);
  void Run();
  ~Server();
 private:

  struct ServerElement {
    int fd;
    sockaddr_in addr;
    ServerElement(int fd, sockaddr_in addr) : fd(fd), addr(addr) {};
  };

  struct ReadElement {
    int fd;
    Request request;
    ReadElement(int fd): fd(fd) { };
  };

  struct WriteElement {
    int fd;
    Request request;
    WriteElement(int fd, Request & request): fd(fd) { this->request = request; };
  };
  const std::vector<ServerConfig> config;
  std::vector<ServerElement> server;
  std::vector<ReadElement> read;
  std::vector<WriteElement> write;
  std::vector<sockaddr_in> server_addr;
  fd_set master_read;
  fd_set working_read;
  fd_set master_write;
  fd_set working_write;
  timeval timeout;
  char *buf;
  const ssize_t INPUT_BUFFER_SIZE;
  int max_fd;
  unsigned status;
  MessageValidator validator_;
  MessageParser parser_;
  void Init();
  Server();
  int Guard(ssize_t retval, bool rw_operation);
  void ConnectionAccept();
  void SocketRead();
  void SocketWrite();
  void ProcessInputBuffer(char * buffer, Request & request);
  void GetHeaders(Request & request);
  void GetBody(Request & request);
  const char * SendResponse(Request& req);

  size_t kek;
  typedef std::vector<ServerElement>::iterator server_iterator;
  typedef std::vector<ReadElement>::iterator read_iterator;
  typedef std::vector<WriteElement>::iterator write_iterator;
  bool isHeader;

};
#endif // WEBSERV_SERVER_HPP_
