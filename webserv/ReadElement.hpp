//
// Created by sfcdo on 20.07.2021.
//

#ifndef WEBSERV_READELEMENT_HPP_
#define WEBSERV_READELEMENT_HPP_
#include "includes/parser.hpp"
#include "Request.hpp"

enum class ReadElementLoggingOptions {
  ZERO,
  REQUEST_BUFFER,
  REQUEST_FULL_BUFFER,
  REQUEST_STATUS_CODE,
  REQUEST_REQUEST_LINE,
  REQUEST_HEADERS,
  REQUEST_TILL_BODY,
  REQUEST_BODY,
  REQUEST_FULL_BODY,
  FULL_REQUEST,
  SERVER,
  CLIENT,
  BUFFER,
  FULL_BUFFER,
  LAST_READ,
  LAST_ACTION_TIME,
  IDLE_TIME,
  ADDR,
  CLIENT_IS_SET
};

class ReadElement: public ILogger<ReadElementLoggingOptions> {
 public:
  ReadElement(const int & server_fd, const int & fd, const ServerConfig & config,
              const sockaddr_in & addr, size_t last_read = Logger::GetTimeInSeconds());
  ~ReadElement();
  const std::string PrintLog(const ReadElementLoggingOptions &option) const;
  const int & GetServerFd() const;
  const int & GetClientFd() const;
  Request & GetRequest();
  const long & GetTimeOfLastRead() const;
  const long & GetTimeOfLastAction() const;
  void SetLastActionTime(const long & time);
  void SetLastReadTime(const long & time);
  void UpdateLastActionSeconds();
  void UpdateLastReadSeconds();
  long GetIdleSeconds() const;
  const std::string & GetBuffer() const;
  void SetBuffer(const std::string & s);
  void AddToBuffer(const std::string & s);
  const sockaddr_in & GetAddr() const;
  void ClearRequest();
 private:
  const int server_fd;
  const int fd;
  std::string buffer;
  Request request;
  long last_read;
  long last_action_time;
  const sockaddr_in addr;
};

#endif //WEBSERV_READELEMENT_HPP_
