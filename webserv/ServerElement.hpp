//
// Created by sfcdo on 20.07.2021.
//

#ifndef WEBSERV_SERVERELEMENT_HPP_
#define WEBSERV_SERVERELEMENT_HPP_
#include "includes/parser.hpp"
#include <netinet/in.h>
#include <set>
class ServerElement {
 public:
  ServerElement(const int &fd, const sockaddr_in &addr, const ServerConfig &server_config);
  void AddClient(const int &fd);
  void RemoveClient(const int &fd);
  const ServerConfig& GetConfig() const;
  const sockaddr_in & GetAddr() const;
  const int & GetServerFd() const;
 private:
  const int server_fd;
  const sockaddr_in addr;
  const ServerConfig &server_config;
  std::set<int> clients;
};

#endif //WEBSERV_SERVERELEMENT_HPP_
