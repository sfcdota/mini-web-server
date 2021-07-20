//
// Created by sfcdo on 20.05.2021.
//

#ifndef WEBSERV_SERVER_HPP_
#define WEBSERV_SERVER_HPP_
#include "Response.hpp"
#include "MessageParser.hpp"
#include "MessageValidator.hpp"
#include "ServerElement.hpp"
#include "ReadElement.hpp"
#include "WriteElement.hpp"
#include "AServer.hpp"
#include "IServer.hpp"
#include "BufferProcessor.hpp"
class Server: AServer, IServer {
 public:
  Server(const std::vector<ServerConfig>& config, const ssize_t & INPUT_BUFFER_SIZE);
  void Run();
  ~Server();
 private:
  std::list<ServerElement> server;
  std::list<ReadElement> read;
  std::list<WriteElement> write;
  unsigned status;
  void Init();
  int Guard(ssize_t retval, bool rw_operation);
  void AcceptConnections();
  void SocketsRead();
  void SocketsWrite();
  void ProcessInputBuffer(char * buffer, Request & request);
  void GetHeaders(Request & request);
  void GetBody(Request & request);
  void GetChunkedBody(Request & request);
  long GetTimeInSeconds();
  void ClearBrokenConnection(int fd);

  template<class Iterator> void PrintLog(Iterator it, const std::string & msg, int client_fd);
  typedef std::list<ServerElement>::iterator server_iterator;
  typedef std::list<ReadElement>::iterator read_iterator;
  typedef std::list<WriteElement>::iterator write_iterator;

};
#endif // WEBSERV_SERVER_HPP_
