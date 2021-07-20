//
// Created by sfcdo on 20.07.2021.
//

#ifndef WEBSERV_ISERVER_HPP_
#define WEBSERV_ISERVER_HPP_

class IServer {
 public:
  virtual void Run() = 0;
  virtual void Initialize() = 0;
  virtual void AcceptConnections() = 0;
  virtual void SocketsRead() = 0;
  virtual void SocketsWrite() = 0;
};
#endif //WEBSERV_ISERVER_HPP_
