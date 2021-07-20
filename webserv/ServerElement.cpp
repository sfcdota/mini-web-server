//
// Created by sfcdo on 20.07.2021.
//

#include <sstream>
#include "ServerElement.hpp"
ServerElement::ServerElement(const int &fd, const sockaddr_in &addr, const ServerConfig &server_config)
    : server_fd(fd), addr(addr), server_config(server_config) {}

ServerElement::~ServerElement() {

}

const std::string ServerElement::PrintLog(const int &logginglevel, const ServerElementLoggingOptions &option) const {
  if (option == ServerElementLoggingOptions::ZERO)
    return std::string();
  std::stringstream ss;
  ss << "Server #" << server_fd << ": ";
  if (option == ServerElementLoggingOptions::CLIENTS) {
    ss << "clients = ";
    for (std::set<int>::const_iterator it = clients.begin(); it != clients.end(); ++it)
       ss << *it << " ";
  }
//  else if (option == ServerElementLoggingOptions::CONFIG)
//    ss << "server config = " << server_config;
  ss << std::endl;
  return ss.str();
}

void ServerElement::AddClient(const int &fd) {
  clients.insert(fd);
}
void ServerElement::RemoveClient(const int &fd) {
  clients.erase(fd);
}
const ServerConfig &ServerElement::GetConfig() const {
  return server_config;
}
const sockaddr_in & ServerElement::GetAddr() const {
  return addr;
}
const int & ServerElement::GetServerFd() const {
  return server_fd;
}
