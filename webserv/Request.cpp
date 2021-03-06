#include "Request.hpp"

Request::Request(const ServerConfig& config, const sockaddr_in & addr, const std::string & buf)
    : ARequest(config), addr(addr), buffer(buf), failed(),
      chunked(), recieved_headers(), recieved_body(), formed(),
      content_length(), last_searched_index(0), cgi_request(false) {}

Request::Request(const Request &in): ARequest(in.server_config), addr(in.addr), buffer(in.buffer)
    { *this = in; }

Request &Request::operator=(const Request &in) {
//  if (!request_line.empty() && request_line.at("target") == "/directory/youpi.bla") {
//    bool hui =true;
//  }
//  if(!request_line.empty())
  ARequest::operator=(in);
  failed = in.failed;
  chunked = in.chunked;
  recieved_headers = in.recieved_headers;
  recieved_body = in.recieved_body;
  formed = in.formed;
  cgi_request = in.cgi_request;
  content_length = in.content_length;
  buffer = in.buffer;
  source_request = in.source_request;
  force_to_break = in.force_to_break;
  last_searched_index = in.last_searched_index;
  return *this;
}

Request::~Request() {}

const std::string Request::PrintLog(const RequestLoggingOptions &option) const {
  if (option == RequestLoggingOptions::ZERO)
    return std::string();
  std::stringstream ss;
  ss << "Request: ";
  if (option == RequestLoggingOptions::BUFFER)
    ss << "request_buffer = " <<  GetShortString(buffer);
  else if (option == RequestLoggingOptions::FULL_BUFFER)
    ss << "full request_buffer = " << buffer;
  else if (option == RequestLoggingOptions::REQUEST_STATUS_CODE)
    ss << "request status code = " << status_code;
  else if (option == RequestLoggingOptions::REQUEST_REQUEST_LINE)
    ss << "request line = " << RequestLineToStr();
  else if (option == RequestLoggingOptions::REQUEST_HEADERS)
    ss << "request headers = " << HeadersToStr();
  else if (option == RequestLoggingOptions::REQUEST_TILL_BODY)
    ss << "request line & headers = " << RequestLineToStr() << HeadersToStr();
  else if (option == RequestLoggingOptions::REQUEST_BODY)
    ss << "request short body = " << GetShortString(body);
  else if (option == RequestLoggingOptions::REQUEST_FULL_BODY)
    ss << "full body = " << body;
  else if (option == RequestLoggingOptions::FULL_REQUEST)
    ss << "full request = " << RequestLineToStr() << HeadersToStr() << GetShortString(body);
  ss << std::endl;
  return ss.str();
}

const std::map<std::string, std::string> &Request::GetRequestLine() const {
  return request_line;
}

const std::map<std::string, std::string> &Request::GetHeaders() const {
  return headers;
}

const std::string &Request::GetBody() const {
  return body;
}

void Request::SetRequestLine(const std::map<std::string, std::string> &request_line) {
  this->request_line = request_line;
}

void Request::SetHeaders(const std::map<std::string, std::string> &headers) {
  this->headers = headers;
}

void Request::SetBody(const std::string &body) {
  this->body = body;
}

void Request::AddHeader(const std::pair<std::string, std::string> pair) {
  headers.insert(pair);
}

const ServerConfig &Request::GetServerConfig() const {
  return server_config;
}

const size_t &Request::GetStatusCode() const {
  return status_code;
}

void Request::SetStatusCode(const size_t &code) {
  status_code = code;
}

const bool &Request::IsCloseOnEnd() const {
  return closeOnEnd;
}

void Request::SetCloseOnEnd(const bool &value) {
  closeOnEnd = value;
}

const sockaddr_in &Request::GetAddr() const {
  return addr;
}

const std::string &Request::GetRequestBuffer() const {
  return buffer;
}

void Request::AppendRequestBuffer(const std::string &s) {
  buffer += s;
}

void Request::SetRequsetBuffer(const std::string &s) {
  buffer = s;
}

const bool &Request::IsFailed() const {
  return failed;
}

const bool &Request::IsRecievedHeaders() const {
  return recieved_headers;
}
void Request::SetRecievedHeaders(const bool &value) {
  recieved_headers = value;
}

const int64_t &Request::GetExpectedContentLength() const {
  return content_length;
}

void Request::SetExpectedContentLength(const int64_t &len) {
  content_length = len;
}

void Request::AppendRequestBody(const std::string &s) {
  body += s;
}

const std::string Request::RequestLineToStr() const {
  std::stringstream ss;
  for(std::map<std::string, std::string>::const_iterator it = request_line.begin(); it != request_line.end(); it++)
      ss << it->first << ": " << it->second << std::endl;
  return ss.str();
}

const std::string Request::HeadersToStr() const {
  std::stringstream ss;
  for(std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); it++)
//    if (it->first == "Content-Length" || it->first == "Transfer-Encoding" || it->first == "Content-Type")
      ss << it->first << ": " << it->second << std::endl;
  return ss.str();
}

void Request::AdjustHeaders() {
//  if (failed)
//    return;



  std::map<std::string, std::string>::iterator content = headers.find("Content-Length");
  std::map<std::string, std::string>::iterator transfer = headers.find("Transfer-Encoding");
  std::map<std::string, std::string>::iterator connection = headers.find("Connection");


  if (request_line["target"].rfind('.') != std::string::npos && request_line["target"].substr(request_line["target"].rfind('.')) == ".bla")
    closeOnEnd = true;
  if (connection != headers.end() && connection->second.find("close") != -1)
    closeOnEnd = true;
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

void Request::SetFailed(const size_t &status_code) {
  failed = true;
  if (status_code && status_code != 200)
    this->status_code = status_code;
  else
    this->status_code = 400;
  formed = true;
}

const bool &Request::IsFormed() const {
  return formed;
}

void Request::SetFormed(const bool &value) {
  formed = value;
}

const bool &Request::IsChunked() const {
  return chunked;
}

void Request::AppendSourceRequest(const std::string &s) {
  source_request += s;
}
const std::string &Request::GetSourceRequest() const {
  return source_request;
}

void Request::Clear() {
  request_line.clear();
  headers.clear();
}

const size_t &Request::GetLastSearchedIndex() const {
  return last_searched_index;
}
void Request::SetLastSearchedIndex(const size_t &value) {
  last_searched_index = value;
}

const bool &Request::GetSendEOF() const {
  return send_eof;
}
void Request::SetSendEOF(const bool &value) {
  send_eof = value;
}

//void Request::CleanUp() {
//  headers.clear();
//  request_line.clear();
//  source_request = std::string();
//  body = std::string();
//  content_length = 0;
//  status_code = 0;
//  formed = false;
//  recieved_headers = false;
//  keep_alive = true;
//}
