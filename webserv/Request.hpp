
#ifndef WEBSERV_REQUEST_HPP_
#define WEBSERV_REQUEST_HPP_
#include "allowed_library_includes.hpp"
#include "../includes/parser.hpp"
#include "MessageValidator.hpp"
class Request {
 public:
  Request(MessageValidator & validator);
  Request(const std::string& buffer, ServerConfig config, MessageValidator & validator);
  Request(const Request & in);
  const Request & operator=(const Request & in);
  const std::string &GetBody() const;
  const std::map<std::string, std::string> &GetRequestLine() const;
  void SetRequestLine(const std::map<std::string, std::string> &request_line);
  const std::map<std::string, std::string> &GetHeaders() const;
  void SetHeaders(const std::map<std::string, std::string> &headers);
  void SetBody(const std::string &body);
  void PrintRequestLine();
  void PrintHeaders();
  void PrintBody();
  void Print();
  void AdjustHeaders();
  void SetFailed(size_t status_code);
  void CleanUp();
// private:
  std::map<std::string, std::string> request_line;
  std::map<std::string,std::string> headers;
  std::string body; //????????
  size_t status_code;
  bool failed;
  bool chunked;
  bool recieved_headers;
  bool recieved_body;
  bool keep_alive;
  bool formed;
  int64_t content_length;
  std::string buffer;
  ServerConfig server_config;
  std::string source_request;
  MessageValidator& validator_;
//	id _request_line;
};
#endif // WEBSERV_REQUEST_HPP_
