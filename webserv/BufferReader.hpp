//
// Created by sfcdo on 20.07.2021.
//

#ifndef WEBSERV_BUFFERREADER_HPP_
#define WEBSERV_BUFFERREADER_HPP_
#include <cstdio>
#include "Request.hpp"
#include "MessageValidator.hpp"
#include "MessageParser.hpp"
#include "ReadElement.hpp"
#include <list>
class BufferReader {
 public:
  BufferReader(const ssize_t & INPUT_BUFFER_SIZE);
  unsigned GetClientMessage(std::list<ReadElement>::iterator & it);
  ~BufferReader();
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
#endif //WEBSERV_BUFFERREADER_HPP_
