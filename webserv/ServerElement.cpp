//
// Created by sfcdo on 20.07.2021.
//

#include "ServerElement.hpp"
ServerElement::ServerElement(const int &fd, const sockaddr_in &addr, const ServerConfig &server_config)
    : server_fd(fd), addr(addr), server_config(server_config) {}

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
