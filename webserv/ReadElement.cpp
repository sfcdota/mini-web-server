//
// Created by sfcdo on 20.07.2021.
//

#include "ReadElement.hpp"

//:
//server_fd(server_fd), fd(fd), last_read(last_read), last_action_time(last_read) {
////      std::cout << "LOCATIONS SIZE" << server_config.locations.size() << std::endl;
//  request.server_config = server_config;
//  request.addr = addr;
//  request.addr_len = len;
//}

ReadElement::ReadElement(const int & server_fd, const int & fd, const ServerConfig & config,
                         const sockaddr_in & addr, size_t last_read)
    : server_fd(server_fd), fd(fd), addr(addr), last_read(last_read), request(config) {}

ReadElement::~ReadElement() {}

long ReadElement::GetTimeInSeconds() {
  gettimeofday(&timev, NULL);
  return timev.tv_sec;
}

const int &ReadElement::GetServerFd() const {
  return server_fd;
}

const int &ReadElement::GetClientFd() const {
  return fd;
}

Request &ReadElement::GetRequest() {
  return request;
}

const long &ReadElement::GetTimeOfLastRead() const {
  return last_read;
}

const long &ReadElement::GetTimeOfLastAction() const {
  return last_action_time;
}

void ReadElement::SetLastActionTime(const long & time) {
  last_action_time = time;
}

void ReadElement::SetLastReadTime(const long & time) {
  last_read = time;
}

void ReadElement::UpdateLastReadSeconds() {
  last_read = GetTimeInSeconds();
}

void ReadElement::UpdateLastActionSeconds() {
  last_action_time = GetTimeInSeconds();
}

long ReadElement::GetIdleSeconds() {
  return last_action_time - last_read;
}


const std::string &ReadElement::GetBuffer() const {
  return buffer;
}
void ReadElement::SetBuffer(const std::string &s) {
  buffer = s;
}
void ReadElement::AddToBuffer(const std::string &s) {
  buffer += s;
}

const sockaddr_in &ReadElement::GetAddr() const {
  return addr;
}

void ReadElement::ClearRequest() {
  request = Request(request.server_config);
}
