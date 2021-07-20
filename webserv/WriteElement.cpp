//
// Created by sfcdo on 20.07.2021.
//

#include "WriteElement.hpp"
WriteElement::WriteElement(const int & server_fd, const int & fd, const bool & closeOnEnd,
                           const std::string & response_msg)
    : server_fd(server_fd), fd(fd), closeOnEnd(closeOnEnd),output(response_msg),
      sent_out_bytes(0) {}

WriteElement::~WriteElement() {}

const int &WriteElement::GetServerFd() {
  return server_fd;
}

const int &WriteElement::GetClientFd() {
  return fd;
}

const std::string &WriteElement::GetOutput() const {
  return output;
}

size_t WriteElement::GetOutLength() const {
  return output.size();
}

const size_t &WriteElement::GetSentBytes() const {
  return sent_out_bytes;
}
void WriteElement::IncreaseSentBytes(const size_t & b) {
  sent_out_bytes += b;
}

const bool &WriteElement::GetCloseOnEnd() const {
  return closeOnEnd;
}
