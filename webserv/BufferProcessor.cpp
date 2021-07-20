//
// Created by sfcdo on 20.07.2021.
//

#include <sstream>
#include "BufferProcessor.hpp"

BufferProcessor::BufferProcessor(const ssize_t &INPUT_BUFFER_SIZE)
    : BUFFER_SIZE_(INPUT_BUFFER_SIZE){
  read_buffer_ = reinterpret_cast<char *>(calloc(INPUT_BUFFER_SIZE, sizeof(char)));
}

unsigned BufferProcessor::GetClientMessage(std::list<ReadElement, std::allocator<ReadElement> >::iterator &it) {
  unsigned status = 0;
  for (; (status = recv(it->GetClientFd(), read_buffer_, BUFFER_SIZE_, 0)) != -1 && status;) {
//    std::stringstream ss;
//        ss << "recv value = " << status  << "|read_buffer_ = [" << read_buffer_ << "]";
//        PrintLog(it, ss.str(), it->fd);
    it->UpdateLastReadSeconds();
//        if (it->request.request_line["method"] == "POST")
//          std::cout << "read_buffer_:"<<read_buffer_ << "$" << std::endl;
    ProcessInputBuffer(it->GetRequest());
    memset(read_buffer_, 0, BUFFER_SIZE_);
//        it->request.PrintRequestLine();
//        std::cout << "Source request was:" << std::setw(90) << it->request.source_request << std::endl;
  }
  return status;
}

void BufferProcessor::ProcessInputBuffer(Request &request) {
  request.buffer += read_buffer_;
  request.source_request += read_buffer_;
  size_t pos;
  if (!request.recieved_headers) {
    if((pos = request.buffer.find("\r\n\r\n", 0)) == std::string::npos)
      return;
    GetHeaders(request);
    request.buffer = request.buffer.substr(pos + 4);
  }
  if (!request.recieved_body) {
//    std::cout << "size of read_buffer_ = " << request.buffer.length() << std::endl;
    if (request.chunked) {
      if ((pos = request.buffer.find("0\r\n\r\n")) == std::string::npos)
        return;
      GetChunkedBody(request);
    }
    else {
      if (request.buffer.length() < request.content_length)
        return;
      GetBody(request);
    }
  }
}

void BufferProcessor::GetHeaders(Request &request) {
  if (validator_.ValidHeaders(request.buffer))
    parser_.ProcessHeaders(request);
  else
    request.SetFailed(validator_.GetStatusCode());
  request.recieved_headers = true;
  request.AdjustHeaders();
}
void BufferProcessor::GetBody(Request &request) {
  request.body = request.buffer.substr(0, request.content_length);
  request.recieved_body = true;
  request.buffer.clear();
}
void BufferProcessor::GetChunkedBody(Request &request) {
//  std::cout << "buffer before body parsing length = " << request.buffer.length() << std::endl;
  request.formed = true;
  if (validator_.ValidBody(request.buffer))
    parser_.ParseBody(request);
  else {
    std::cout << "VALIDATOR ENCOUNTERED INVALID CHUNKED BODY. REQUEST FAILED" << std::endl;
    request.SetFailed(validator_.GetStatusCode());
  }
  if (request.failed)
    std::cout << "FAILED !!!!!!! " << std::endl;
  std::stringstream ss;
  ss << request.body.length();
  request.headers.insert(std::make_pair("Content-Length", ss.str()));
  request.recieved_body = true;
  request.buffer.clear();
}

BufferProcessor::~BufferProcessor() {
  free(read_buffer_);
}
