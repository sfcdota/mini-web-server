//
// Created by sfcdo on 20.07.2021.
//

#ifndef WEBSERV_WRITEELEMENT_HPP_
#define WEBSERV_WRITEELEMENT_HPP_
#include <string>
#include "Logger.hpp"
enum class WriteElementLoggingOptions {
  ZERO,
  SERVER,
  CLIENT,
  OUTPUT,
  FULL_OUTPUT,
  OUTPUT_SIZE,
  SENT_BYTES,
  CLOSE_ON_END,
  SENDING_PROCESS,
  CLOSING_ON_END,
};

class WriteElement: ILogger<WriteElementLoggingOptions> {
 public:
  WriteElement(const int & server_fd, const int & fd, const bool & closeOnEnd, const std::string & response_msg);
  ~WriteElement();
  const std::string PrintLog(const int & logginglevel, const WriteElementLoggingOptions & option) const;
  const int & GetServerFd();
  const int & GetClientFd();
  const std::string & GetOutput() const;
  size_t GetOutLength() const;
  const size_t & GetSentBytes() const;
  void IncreaseSentBytes(const size_t & b);
  const bool & GetCloseOnEnd() const;
 private:
  const int server_fd;
  const int fd;
  const std::string output;
  size_t sent_out_bytes;
  const bool closeOnEnd;
};

#endif //WEBSERV_WRITEELEMENT_HPP_
