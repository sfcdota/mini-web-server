
#ifndef WEBSERV_REQUEST_HPP_
#define WEBSERV_REQUEST_HPP_
#include <netinet/in.h>
#include "../includes/parser.hpp"
#include "Abstract&Interfaces/ARequest.hpp"
#include <iostream>

class Request: ARequest {
 public:
  Request(const ServerConfig& config, const sockaddr_in & addr, const std::string & buf = std::string());
  Request(const Request & in);
  Request & operator=(const Request & in);
  const std::map<std::string, std::string> &GetRequestLine() const;
  const std::map<std::string, std::string> &GetHeaders() const;
  const std::string &GetBody() const;
  void SetRequestLine(const std::map<std::string, std::string> &request_line);
  void SetHeaders(const std::map<std::string, std::string> &headers);
  void SetBody(const std::string &body);
  void AddHeader(const std::pair<std::string, std::string> pair);
  const ServerConfig& GetServerConfig() const;
  const size_t & GetStatusCode() const;
  void SetStatusCode(const size_t & code);
  const bool & IsCloseOnEnd() const;
  void SetCloseOnEnd(const bool & value);
  const sockaddr_in & GetAddr() const;
  void PrintRequestLine() const;
  void PrintHeaders() const;
  void PrintBody() const;
  void Print() const;
  const std::string & GetRequestBuffer() const;
  void AppendRequestBuffer(const std::string & s);
  void SetRequsetBuffer(const std::string & s);
  void AppendRequestBody(const std::string & s);
  void AdjustHeaders();
  void SetFailed(const size_t & status_code);
  const bool & IsFormed() const;
  void SetFormed(const bool & value);
  const bool & IsChunked() const;
  const bool & IsFailed() const;
  const bool & IsRecievedHeaders() const;
  void SetRecievedHeaders(const bool & value);
  const int64_t & GetExpectedContentLength() const;
  ///todo for debug only
  void AppendSourceRequest(const std::string & s);
  const std::string & GetSourceRequest() const;

 private:
  bool failed;
  bool chunked;
  bool recieved_headers;
  bool recieved_body;
  bool formed;
  bool cgi_request;
  int64_t content_length;
  std::string buffer;
  std::string source_request;
  const sockaddr_in addr;
  bool force_to_break;
};
#endif // WEBSERV_REQUEST_HPP_
