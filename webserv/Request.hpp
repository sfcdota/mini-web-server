
#ifndef WEBSERV_REQUEST_HPP_
#define WEBSERV_REQUEST_HPP_
#include "allowed_library_includes.hpp"
#include "../includes/parser.hpp"

class Request {
 public:
  Request();
  Request(const std::string& buffer, ServerConfig config);
  Request(const Request & in);
  Request & operator=(const Request & in);
  const std::string &GetBody() const;
  const string_map &GetRequestLine() const;
  void SetRequestLine(const string_map &request_line);
  const string_map &GetHeaders() const;
  void SetHeaders(const string_map &headers);
  void SetBody(const std::string &body);
  void PrintRequestLine();
  void PrintHeaders();
  void PrintBody();
  void Print();
  void AdjustHeaders();
  void SetFailed(size_t status_code);
  void CleanUp();
// private:
  string_map request_line;
  string_map headers;
  string_map cgi_headers;
  std::string body; //????????
  size_t status_code;
  bool failed;
  bool chunked;
  bool recieved_headers;
  bool recieved_body;
  bool keep_alive;
  bool formed;
  bool force_to_break;
  int64_t content_length;
  std::string buffer;
  ServerConfig server_config;
  std::string source_request;
  std::string target_absolute_path;
  std::string target_relative_path;
  std::string target_absolute_path_wo_query;
  std::string target_relative_path_wo_query;
//	id _request_line;
};
#endif // WEBSERV_REQUEST_HPP_
