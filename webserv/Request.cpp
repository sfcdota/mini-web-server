
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
Request::Request(): keep_alive(true), status_code(), failed(), chunked(), recieved_headers(), recieved_body(), formed(), content_length() {}

Request::Request(const std::string &buffer, ServerConfig config): keep_alive(true), buffer(buffer),
  server_config(config), status_code(), failed(), chunked(), recieved_headers(), recieved_body(), formed(),
  content_length(), force_to_break(false) {}

Request::Request(const Request &in) : status_code(), failed(), chunked(), recieved_headers(), recieved_body(), formed(), keep_alive(), content_length() { *this = in; }
Request &Request::operator=(const Request &in) {
  this->content_length = in.content_length;
  this->keep_alive = in.keep_alive;
  this->request_line = in.request_line;
  this->headers = in.headers;
  this->body = in.body;
  this->status_code = in.status_code;
  this->failed = in.failed;
  this->chunked = in.chunked;
  this->recieved_headers = in.recieved_headers;
  this->formed = in.formed;
  this->buffer = in.buffer;
  this->server_config = in.server_config;
  this->source_request = in.source_request;
  this->recieved_body = in.recieved_body;

  return *this;
}
void Request::PrintRequestLine() {
  std::cout << "%%%%%%%%%%%%%%%%% REQUEST LINE %%%%%%%%%%%%%%%%%" << std::endl;
  for(std::map<std::string, std::string>::iterator it = request_line.begin(); it != request_line.end(); it++)
      std::cout << it->second << ' ';
  std::cout << std::endl;
  std::cout << "%%%%%%%%%%%%%%%%% END OF REQUEST LINE %%%%%%%%%%%%%%%%%" << std::endl;
}
void Request::PrintHeaders() {
  std::cout << "$$$$$$$$$$$$$$$$ HEADERS BLOCK $$$$$$$$$$$$$$$$" << std::endl;
  for(std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
    if (it->first == "Content-Length" || it->first == "Transfer-Encoding" || it->first == "Content-Type")
      std::cout << it->first << ":" << it->second << std::endl;
  std::cout << "$$$$$$$$$$$$$$$$ END OF REQUEST LINE $$$$$$$$$$$$$$$$" << std::endl;
}

void Request::PrintBody() {
  std::cout << "#################### BODY ####################" <<std::endl << body
  << "#################### END OF BODY ####################" << std::endl;
}

void Request::Print() {
  std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ FORMED REQUEST @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
  PrintRequestLine();
  PrintHeaders();
//  PrintBody();
  std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ END OF FORMED REQUEST @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
}

const std::string &Request::GetBody() const {
  return body;
}

void Request::AdjustHeaders() {
  std::map<std::string, std::string>::iterator content = headers.find("Content-Length");
  std::map<std::string, std::string>::iterator transfer = headers.find("Transfer-Encoding");
  std::map<std::string, std::string>::iterator connection = headers.find("Connection");

  if (connection != headers.end() && connection->second.find("close") != -1)
    keep_alive = false;
  if (content == transfer) {
    recieved_body = true;
    formed = true;
    return;
  }
  if (transfer != headers.end() && transfer->second.find("chunked") != -1)
    chunked = true;
  if (content != headers.end()) {
    if (transfer != headers.end())
      headers.erase(content);
    else {
      std::cout << "HEADER | " << content->first << ": " << content->second << std::endl;
      content_length = strtol(content->second.c_str(), NULL, 0);
      std::cout << "PARSED CONTENT LENGTH | " << content_length << std::endl;
      if (content_length < 0)
        SetFailed(400);
      if (content_length > server_config.client_max_body_size)
        SetFailed(413);
    }
  }
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
  headers.clear();
  request_line.clear();
  source_request = std::string();
  body = std::string();
  content_length = 0;
  status_code = 0;
  formed = false;
  recieved_headers = false;
  keep_alive = true;
}
