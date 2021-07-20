//
// Created by sfcdo on 20.07.2021.
//

#ifndef WEBSERV_ISERVER_HPP_
#define WEBSERV_ISERVER_HPP_

class IServer {
 public:
  virtual void Run() const = 0;
  virtual void Initialize() const = 0;
  virtual void AcceptConnections() const = 0;
  virtual void SocketsRead() const = 0;
  virtual void SocketsWrite() const = 0;
};
#endif //WEBSERV_ISERVER_HPP_
