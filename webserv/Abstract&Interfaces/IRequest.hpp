
#ifndef WEBSERV_ABSTRACT_INTERFACES_IREQUEST_HPP_
#define WEBSERV_ABSTRACT_INTERFACES_IREQUEST_HPP_

class IRequest {
 public:
  virtual const std::map<std::string, std::string> &GetRequestLine() const = 0;
  virtual const std::map<std::string, std::string> &GetHeaders() const = 0;
  virtual const std::string & GetBody() const = 0;
  virtual void SetRequestLine(const std::map<std::string, std::string> &request_line) = 0;
  virtual void SetHeaders(const std::map<std::string, std::string> &headers) = 0;
  virtual void SetBody(const std::string & body) = 0;
  virtual const ServerConfig & GetServerConfig() const = 0;
  virtual const size_t & GetStatusCode() const = 0;
  virtual void SetStatusCode(const size_t & code) = 0;
  virtual const bool & IsCloseOnEnd() const = 0;
  virtual void SetCloseOnEnd(const bool & value) = 0;
};
#endif
