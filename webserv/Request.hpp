
#ifndef WEBSERV_REQUEST_HPP_
#define WEBSERV_REQUEST_HPP_
#include <netinet/in.h>
#include <sstream>
#include "../includes/parser.hpp"
#include "Abstract&Interfaces/ARequest.hpp"
#include "Logger.hpp"
#include "IRequest.hpp"
enum class RequestLoggingOptions {
  ZERO,
  BUFFER,
  FULL_BUFFER,
  REQUEST_STATUS_CODE,
  REQUEST_REQUEST_LINE,
  REQUEST_HEADERS,
  REQUEST_TILL_BODY,
  REQUEST_BODY,
  REQUEST_FULL_BODY,
  FULL_REQUEST
};

class Request: public ARequest, IRequest, public ILogger<RequestLoggingOptions> {
 public:
  Request(const ServerConfig& config, const sockaddr_in & addr, const std::string & buf = std::string());
  Request(const Request & in);
  Request & operator=(const Request & in);
  virtual ~Request();
  const std::string PrintLog(const RequestLoggingOptions &option) const;
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
  const std::string RequestLineToStr() const;
  const std::string HeadersToStr() const;
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
  void Clear();

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
