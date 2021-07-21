//
// Created by sfcdo on 20.07.2021.
//

#include <sstream>
#include "ReadElement.hpp"
#include <arpa/inet.h>
//:
//server_fd(server_fd), fd(fd), last_read(last_read), last_action_time(last_read) {
////      std::cout << "LOCATIONS SIZE" << server_config.locations.size() << std::endl;
//  request.server_config = server_config;
//  request.addr = addr;
//  request.addr_len = len;
//}

ReadElement::ReadElement(const int & server_fd, const int & fd, const ServerConfig & config,
                         const sockaddr_in & addr, size_t last_read)
    : server_fd(server_fd), fd(fd), addr(addr), last_read(last_read), request(config, addr) {

}

ReadElement::~ReadElement() {
//  delete request;
}

const std::string ReadElement::PrintLog(const ReadElementLoggingOptions &option) const {
  if (option == ReadElementLoggingOptions::ZERO)
    return std::string();
  std::stringstream ss;
  ss << "ReadElement: ";
  if (option == ReadElementLoggingOptions::REQUEST_BUFFER)
    ss << request.PrintLog(RequestLoggingOptions::BUFFER);
  else if (option == ReadElementLoggingOptions::REQUEST_FULL_BUFFER)
    ss << request.PrintLog(RequestLoggingOptions::FULL_BUFFER);
  else if (option == ReadElementLoggingOptions::REQUEST_STATUS_CODE)
    ss << request.PrintLog(RequestLoggingOptions::REQUEST_STATUS_CODE);
  else if (option == ReadElementLoggingOptions::REQUEST_REQUEST_LINE)
    ss << request.PrintLog(RequestLoggingOptions::REQUEST_REQUEST_LINE);
  else if (option == ReadElementLoggingOptions::REQUEST_HEADERS)
    ss << request.PrintLog(RequestLoggingOptions::REQUEST_HEADERS);
  else if (option == ReadElementLoggingOptions::REQUEST_TILL_BODY)
    ss << request.PrintLog(RequestLoggingOptions::REQUEST_TILL_BODY);
  else if (option == ReadElementLoggingOptions::REQUEST_BODY)
    ss << request.PrintLog(RequestLoggingOptions::REQUEST_BODY);
  else if (option == ReadElementLoggingOptions::REQUEST_FULL_BODY)
    ss << request.PrintLog(RequestLoggingOptions::REQUEST_FULL_BODY);
  else if (option == ReadElementLoggingOptions::FULL_REQUEST)
    ss << request.PrintLog(RequestLoggingOptions::FULL_REQUEST);
  else if (option == ReadElementLoggingOptions::SERVER)
    ss << "server fd = " << server_fd;
  else if (option == ReadElementLoggingOptions::CLIENT)
    ss << "client fd = " << fd;
  else if (option == ReadElementLoggingOptions::BUFFER)
    ss << "short buffer = " << GetShortString(buffer);
  else if (option == ReadElementLoggingOptions::FULL_BUFFER)
    ss << "full buffer = " << buffer;
  else if (option == ReadElementLoggingOptions::LAST_READ)
    ss << "last read time = " << last_read;
  else if (option == ReadElementLoggingOptions::LAST_ACTION_TIME)
    ss << "last action time = " << last_action_time;
  else if (option == ReadElementLoggingOptions::IDLE_TIME)
    ss << "client #" << fd << " idle seconds = " << GetIdleSeconds();
  else if (option == ReadElementLoggingOptions::ADDR)
    ss << "client #" << fd << " addr = " << inet_ntoa(addr.sin_addr);
  else if (option == ReadElementLoggingOptions::CLIENT_IS_SET)
    ss << "client #" << fd << " IS SET FOR READ";
  ss << std::endl;
  return ss.str();
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
  last_read = Logger::GetTimeInSeconds();
}

void ReadElement::UpdateLastActionSeconds() {
  last_action_time = Logger::GetTimeInSeconds();
}

long ReadElement::GetIdleSeconds() const {
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
//  request(request.GetServerConfig(), addr);
//  request.Clear();
  const ServerConfig& hui = request.GetServerConfig();
//  delete request;
  request =  Request(hui, addr, buffer);

}
