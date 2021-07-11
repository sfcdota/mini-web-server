//
// Created by sfcdo on 20.05.2021.
//

#ifndef WEBSERV_SERVER_HPP_
#define WEBSERV_SERVER_HPP_
#include "Response.hpp"
#include "MessageParser.hpp"
#include "MessageValidator.hpp"

class Server {
 public:
  Server(const std::vector<ServerConfig>& config, ssize_t INPUT_BUFFER_SIZE);
  void Run();
  ~Server();
 private:

  struct ServerElement {
    int server_fd;
    sockaddr_in addr;
    ServerConfig server_config;
    ServerElement(int fd, sockaddr_in addr, const ServerConfig &server_config):
      server_fd(fd), addr(addr), server_config(server_config) {}
  };

  struct timeval timev;

  struct ReadElement {
    int server_fd;
    int fd;
    Request request;
    size_t last_read;
    size_t last_action_time;
    ReadElement(int server_fd, int fd, const ServerConfig& server_config, size_t last_read = 0):
      server_fd(server_fd), fd(fd), last_read(last_read), last_action_time(last_read) {
      request.server_config = server_config;
    }
  };

  struct WriteElement {
    int server_fd;
    int fd;
    Request request;
    const char* output;
    size_t out_length;
    size_t send_out_bytes;
    WriteElement(int server_fd, int fd, Request & request): server_fd(server_fd), fd(fd), request(request) {
      Response response(request);
      const char * tmp = request.source_request.c_str();
      output = strdup(response.SetResponseLine().c_str());
      out_length = strlen(output);
      send_out_bytes = 0;
    }
    ~WriteElement() {
//      std::cout << "destructing output with value" << std::endl << output << std::endl;
//      free( (void *)output);
    }
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

  template<class Iterator> void PrintLog(Iterator it, const std::string & msg, int client_fd);

  size_t kek;
  typedef std::vector<ServerElement>::iterator server_iterator;
  typedef std::vector<ReadElement>::iterator read_iterator;
  typedef std::vector<WriteElement>::iterator write_iterator;
  bool isHeader;

};
#endif // WEBSERV_SERVER_HPP_
