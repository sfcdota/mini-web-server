//
// Created by sfcdo on 20.07.2021.
//

#ifndef WEBSERV_BUFFERPROCESSOR_HPP_
#define WEBSERV_BUFFERPROCESSOR_HPP_
#include <cstdio>
#include "Request.hpp"
#include "MessageValidator.hpp"
#include "MessageParser.hpp"
#include "ReadElement.hpp"
class BufferProcessor {
 public:
  BufferProcessor(const ssize_t & INPUT_BUFFER_SIZE);
  unsigned GetClientMessage(std::list<ReadElement>::iterator & it);
  ~BufferProcessor();
 private:
  void ProcessInputBuffer(Request & request);
  void GetHeaders(Request & request);
  void GetBody(Request & request);
  void GetChunkedBody(Request & request);
  const ssize_t BUFFER_SIZE_;
  char *read_buffer_;
  MessageValidator validator_;
  MessageParser parser_;
};
#endif //WEBSERV_BUFFERPROCESSOR_HPP_
