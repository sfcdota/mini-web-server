
#include "Request.hpp"

const std::map<std::string, std::string> &Request::GetRequestLine() const {
  return request_line;
}
void Request::SetRequestLine(const std::map<std::string, std::string> &request_line) {
  this->request_line = request_line;
}
const std::map<std::string, std::string> &Request::GetHeaders() const {
  return headers;
}
void Request::SetHeaders(const std::map<std::string, std::string> &headers) {
  this->headers = headers;
}
void Request::SetBody(const std::string &body) {
  this->body = body;
}
Request::Request(): failed(false), status_code(0), formed(false), headersReady(false), keep_alive(true) {}

Request::Request(const std::string &buffer, ServerConfig config): failed(false), status_code(0), formed(false), headersReady(false),
  keep_alive(true), buffer(buffer),
                                                                  server_config(config) {}

Request::Request(const Request &in) { *this = in; }
Request &Request::operator=(const Request &in) {
  this->keep_alive = in.keep_alive;
  this->request_line = in.request_line;
  this->headers = in.headers;
  this->body = in.body;
  this->status_code = in.status_code;
  this->failed = in.failed;
  this->chunked = in.chunked;
  this->headersReady = in.headersReady;
  this->formed = in.formed;
  this->buffer = in.buffer;
  this->server_config = in.server_config;
  this->source_request = in.source_request;
  return *this;
}
void Request::PrintRequestLine() {
  std::cout << "%%%%%%%%%%%%%%%%% REQUEST LINE %%%%%%%%%%%%%%%%%" << std::endl;
  for(std::map<std::string, std::string>::iterator it = request_line.begin(); it != request_line.end(); it++)
      std::cout << it->first << "=" << it->second << std::endl;
  std::cout << "%%%%%%%%%%%%%%%%% END OF REQUEST LINE %%%%%%%%%%%%%%%%%" << std::endl;
}
void Request::PrintHeaders() {
  std::cout << "$$$$$$$$$$$$$$$$ HEADERS BLOCK $$$$$$$$$$$$$$$$" << std::endl;
  for(std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
    std::cout << it->first << ":" << it->second << std::endl;
  std::cout << "$$$$$$$$$$$$$$$$ END OF REQUEST LINE $$$$$$$$$$$$$$$$" << std::endl;
}

void Request::PrintBody() {
  std::cout << "#################### BODY ####################" <<std::endl << body
  << "#################### END OF BODY ####################" << std::endl;
}


const std::string &Request::GetBody() const {
  return body;
}

void Request::AdjustHeaders() {
  std::map<std::string, std::string>::iterator content = headers.find("Content-Length");
  std::map<std::string, std::string>::iterator transfer = headers.find("Transfer-Encoding");
  std::map<std::string, std::string>::iterator connection = headers.find("Connection");

  if (content == transfer)
    formed = true;
  if (transfer != headers.end() && transfer->second.find("chunked") != -1) {
    chunked = true;
  }
  if (content != headers.end()) {
    if (transfer != headers.end())
      headers.erase(content);
  } else {
    content_length = strtol(content->second.c_str(), NULL, 0);
    if (content_length < 0)
      SetFailed(400);
  }
  if (connection != headers.end() && connection->second.find("close") != -1)
    keep_alive = false;
}

void Request::SetFailed(size_t status_code) {
  failed = true;
  if (status_code)
    this->status_code = status_code;
  else
    this->status_code = 400;
  formed = true;
}

void Request::CleanUp() {
  headers = std::map<std::string, std::string>();
  request_line = std::map<std::string, std::string>();
  source_request = std::string();
  body = std::string();
  content_length = 0;
  status_code = 0;

}
