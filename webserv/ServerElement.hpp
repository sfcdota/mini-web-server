//
// Created by sfcdo on 20.07.2021.
//

#ifndef WEBSERV_SERVERELEMENT_HPP_
#define WEBSERV_SERVERELEMENT_HPP_
#include "includes/parser.hpp"
#include <netinet/in.h>
#include <set>
#include "Abstract&Interfaces/ILogger.hpp"

enum class ServerElementLoggingOptions {
  ZERO,
  CLIENTS,
  CONFIG
};

class ServerElement: public ILogger<ServerElementLoggingOptions> {
 public:
  ServerElement(const int &fd, const sockaddr_in &addr, const ServerConfig &server_config);
  ~ServerElement();
  const std::string PrintLog(const ServerElementLoggingOptions &option) const;
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
