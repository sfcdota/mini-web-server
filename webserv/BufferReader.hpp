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

enum class BufferReaderLoggingOptions {
  ZERO,
  BUFFER,
  FULL_BUFFER,
  REQUEST_STATUS_CODE
};

class BufferReader: ILogger<BufferReaderLoggingOptions> {
 public:
  BufferReader(const ssize_t & INPUT_BUFFER_SIZE);
  ~BufferReader();
  const std::string PrintLog(const int & logginglevel, const BufferReaderLoggingOptions & option) const;
  unsigned GetClientMessage(std::list<ReadElement>::iterator & it);
 private:
  void ProcessInputBuffer(Request & request);
  void FillRequestHeaders(Request & request);
  void FillRequestBody(Request & request);
  void FillRequestChunkedBody(Request & request);
  const ssize_t BUFFER_SIZE_;
  char *read_buffer_;
  MessageValidator validator_;
  MessageParser parser_;
};
#endif //WEBSERV_BUFFERREADER_HPP_
