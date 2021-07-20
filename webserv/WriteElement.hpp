//
// Created by sfcdo on 20.07.2021.
//

#ifndef WEBSERV_WRITEELEMENT_HPP_
#define WEBSERV_WRITEELEMENT_HPP_
#include <string>
class WriteElement {
 public:
  WriteElement(const int & server_fd, const int & fd, const bool & closeOnEnd, const std::string & response_msg);
  ~WriteElement();
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
