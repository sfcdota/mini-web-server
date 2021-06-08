
#ifndef WEBSERV_REQUEST_HPP_
#define WEBSERV_REQUEST_HPP_
#include "allowed_library_includes.hpp"
#include "simplifiers.hpp"


class Request {
 public:
  Request();
  const string &GetBody() const;
  const std::map<std::string, std::string> &GetRequestLine() const;
  void SetRequestLine(const std::map<std::string, std::string> &request_line);
  const std::map<std::string, std::string> &GetHeaders() const;
  void SetHeaders(const std::map<std::string, std::string> &headers);
  void SetBody(const string &body);
  void PrintRequestLine();
  void PrintHeaders();
  void PrintBody();
// private:
  std::map<std::string, std::string> request_line;
  std::map<std::string,std::string> headers;
  std::string body; //????????
  size_t status_code;
  bool failed;
};
#endif // WEBSERV_REQUEST_HPP_
