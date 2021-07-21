//
// Created by sfcdo on 20.05.2021.
//

#ifndef WEBSERV_SERVER_HPP_
#define WEBSERV_SERVER_HPP_
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sstream>
#include "Response.hpp"
#include "ServerElement.hpp"
#include "ReadElement.hpp"
#include "WriteElement.hpp"
#include "Abstract&Interfaces/AServer.hpp"
#include "Abstract&Interfaces/IServer.hpp"
#include "BufferReader.hpp"
#include "Logger.hpp"
#define TIMOUT_SEC 15
#define MAX_CONNECTIONS 1024
#define DEFAULT_INPUT_BUFFERSIZE 65536

enum class ServerLoggingOptions {
  ZERO,
  TIMEOUT,
  SERVERELEMENT_COUNT,
  READELEMENT_COUNT,
  WRITELEMENT_COUNT,
  EVERYELEMENT_COUNT,
  STATUS,
  INITIALIZING,
  ACCEPTING,
  READING,
  WRITING,
};

class Server: public AServer, IServer, ILogger<ServerLoggingOptions> {
 public:
  Server(const std::vector<ServerConfig>& config, const ssize_t & INPUT_BUFFER_SIZE);
  ~Server();
  const std::string PrintLog(const ServerLoggingOptions &option) const;
  void Run();
 private:
  std::list<ServerElement> server;
  std::list<ReadElement> read;
  std::list<WriteElement> write;
  unsigned status;
  void Initialize();
  int Guard(ssize_t retval, bool rw_operation);
  void AcceptConnections();
  void SocketsRead();
  void SocketsWrite();
  void ClearBrokenConnection(int fd);

  typedef std::list<ServerElement>::iterator server_iterator;
  typedef std::list<ReadElement>::iterator read_iterator;
  typedef std::list<WriteElement>::iterator write_iterator;
};
#endif // WEBSERV_SERVER_HPP_
