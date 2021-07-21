//
// Created by sfcdo on 20.07.2021.
//

#include <sstream>
#include "WriteElement.hpp"
WriteElement::WriteElement(const int & server_fd, const int & fd, const bool & closeOnEnd,
                           const std::string & response_msg)
    : server_fd(server_fd), fd(fd), closeOnEnd(closeOnEnd), output(response_msg),
      sent_out_bytes(0) {}

WriteElement::~WriteElement() {}

const std::string WriteElement::PrintLog(const WriteElementLoggingOptions &option) const {
  if (option == WriteElementLoggingOptions::ZERO)
    return std::string();
  std::stringstream ss;
  ss << "WriteElement: ";
  if (option == WriteElementLoggingOptions::SERVER)
    ss << "server = " << server_fd;
  else if (option == WriteElementLoggingOptions::CLIENT)
    ss << "client = " << fd;
  else if (option == WriteElementLoggingOptions::OUTPUT)
    ss << "output = " << GetShortString(&output.c_str()[sent_out_bytes]);
  else if (option == WriteElementLoggingOptions::FULL_OUTPUT)
    ss << "output = " << &output.c_str()[sent_out_bytes];
  else if (option == WriteElementLoggingOptions::OUTPUT_SIZE)
    ss << "output size = " << output.size();
  else if (option == WriteElementLoggingOptions::SENT_BYTES)
    ss << "sent bytes = [" << sent_out_bytes << "/" << output.size() << "]";
  else if (option == WriteElementLoggingOptions::CLOSE_ON_END)
    ss << "close on end = " << (closeOnEnd ? "true" : "false");
  else if (option == WriteElementLoggingOptions::SENDING_PROCESS)
    ss << "iteration on sending response with size = " << output.size() << "...";
  else if (option == WriteElementLoggingOptions::CLOSING_ON_END)
    ss << "closing connection with client #" << fd << "| close on end = " << (closeOnEnd ? "true" : "false");
  ss << std::endl;
  return ss.str();
}

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
