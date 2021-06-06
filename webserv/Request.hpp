
#ifndef WEBSERV_REQUEST_HPP_
#define WEBSERV_REQUEST_HPP_
#include "allowed_library_includes.hpp"
#include "simplifiers.hpp"


class Request {
 public:
  Request();
  const std::map<std::string, std::string> &GetRequestLine() const;
  const std::map<std::string, std::string> &GetHeaders() const;
  const string &GetBody() const;
  size_t GetStatusCode() const;
  void AddRequestLineField(const std::pair<std::string, std::string> &request_line);
  void AddHeader(const std::pair<std::string, std::string> &headers);
  void SetBody(const string &body);
  void SetStatusCode(size_t status_code);
 private:
  std::map<std::string, std::string> request_line;
  std::map<std::string,std::string> headers;
  std::string body; //????????
  size_t status_code;
  bool failed;
};
#endif // WEBSERV_REQUEST_HPP_
