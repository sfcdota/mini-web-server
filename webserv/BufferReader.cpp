//
// Created by sfcdo on 20.07.2021.
//

#include <sstream>
#include "BufferReader.hpp"

BufferReader::BufferReader(const ssize_t &INPUT_BUFFER_SIZE)
    : BUFFER_SIZE_(INPUT_BUFFER_SIZE) {
  read_buffer_ = reinterpret_cast<char *>(calloc(INPUT_BUFFER_SIZE, sizeof(char)));
}

const std::string BufferReader::PrintLog(const BufferReaderLoggingOptions &option) const {
//  switch (option) {
//    case BufferReaderLoggingOptions::ZERO:break;
//    case BufferReaderLoggingOptions::BUFFER_SIZE:break;
//    case BufferReaderLoggingOptions::BUFFER:break;
//    case BufferReaderLoggingOptions::FULL_BUFFER:break;
//    case BufferReaderLoggingOptions::READ_BUFFER:break;
//    case BufferReaderLoggingOptions::REQUEST_STATUS_CODE:break;
//    case BufferReaderLoggingOptions::REQUEST_REQUEST_LINE:break;
//    case BufferReaderLoggingOptions::REQUEST_HEADERS:break;
//    case BufferReaderLoggingOptions::REQUEST_TILL_BODY:break;
//    case BufferReaderLoggingOptions::REQUEST_BODY:break;
//    case BufferReaderLoggingOptions::FULL_REQUEST:break;
//  }
  if (option == BufferReaderLoggingOptions::ZERO)
    return std::string();
  std::stringstream ss;
  ss << "BufferReader: ";
  if (option == BufferReaderLoggingOptions::BUFFER)
    ss << "read_buffer = " <<  std::string(read_buffer_).substr(0, 100) << "...";
  else if (option == BufferReaderLoggingOptions::FULL_BUFFER)
    ss << "full read_buffer = " << read_buffer_;
  else if (option == BufferReaderLoggingOptions::REQUEST_STATUS_CODE)
    ss << "validator status code = " << validator_.GetStatusCode();
  ss << std::endl;
  return ss.str();
}

unsigned BufferReader::GetClientMessage(std::list<ReadElement, std::allocator<ReadElement> >::iterator &it) {
  unsigned status = 0;
  for (; (status = recv(it->GetClientFd(), read_buffer_, BUFFER_SIZE_, 0)) != -1 && status;) {
    it->UpdateLastReadSeconds();
    ProcessInputBuffer(it->GetRequest());
    memset(read_buffer_, 0, BUFFER_SIZE_);
  }
  return status;
}

void BufferReader::ProcessInputBuffer(Request &request) {
  request.AppendRequestBuffer(read_buffer_);
  request.AppendSourceRequest(read_buffer_);
  size_t pos;
  if (!request.IsRecievedHeaders()) {
    if((pos = request.GetRequestBuffer().find("\r\n\r\n", 0)) == std::string::npos)
      return;
    FillRequestHeaders(request);
    request.SetRequsetBuffer(request.GetRequestBuffer().substr(pos + 4));
  }
  if (request.IsRecievedHeaders() && !request.IsFormed()) {
    if (request.IsChunked()) {
      if ((pos = request.GetRequestBuffer().find("0\r\n\r\n")) == std::string::npos)
        return;
      FillRequestChunkedBody(request);
    }
    else {
      if (request.GetRequestBuffer().length() < request.GetExpectedContentLength())
        return;
      FillRequestBody(request);
    }
  }
}

void BufferReader::FillRequestHeaders(Request &request) {
  if (validator_.ValidHeaders(request.GetRequestBuffer()))
    parser_.ProcessHeaders(request);
  else
    request.SetFailed(validator_.GetStatusCode());
  request.SetRecievedHeaders(true);
  request.AdjustHeaders();
}
void BufferReader::FillRequestBody(Request &request) {
  request.SetBody(request.GetRequestBuffer().substr(0, request.GetExpectedContentLength()));
  request.SetFormed(true);
  request.SetRequsetBuffer(std::string());
}
void BufferReader::FillRequestChunkedBody(Request &request) {
//  std::cout << "buffer before body parsing length = " << request.buffer.length() << std::endl;
  if (MessageValidator::ValidBody(request.GetRequestBuffer()))
    parser_.ParseBody(request);
  else {
    std::cout << "VALIDATOR ENCOUNTERED INVALID CHUNKED BODY. REQUEST FAILED" << std::endl;
    request.SetFailed(validator_.GetStatusCode());
  }
  if (request.IsFailed())
    std::cout << "FAILED !!!!!!! " << std::endl;
  request.SetExpectedContentLength(request.GetBody().length());
  std::stringstream ss;
  ss << request.GetBody().length();
  request.AddHeader(std::make_pair("Content-Length", ss.str()));
  request.SetFormed(true);
  request.SetRequsetBuffer(std::string());
}

BufferReader::~BufferReader() {
  free(read_buffer_);
}
