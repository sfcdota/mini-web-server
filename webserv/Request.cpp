
#include "Request.hpp"

const std::map<std::string, std::string> &Request::GetRequestLine() const {
  return request_line;
}
const std::map<std::string, std::string> &Request::GetHeaders() const {
  return headers;
}
const string &Request::GetBody() const {
  return body;
}
size_t Request::GetStatusCode() const {
  return status_code;
}
void Request::AddRequestLineField(const std::pair<std::string, std::string> &rlf) {
  Request::request_line.insert(rlf);
}
void Request::AddHeader(const std::pair<std::string, std::string> &header) {
  Request::headers.insert(header);
}
void Request::SetBody(const string &body) {
  Request::body = body;
}
void Request::SetStatusCode(size_t status_code) {
  Request::status_code = status_code;
}
Request::Request() {

}
