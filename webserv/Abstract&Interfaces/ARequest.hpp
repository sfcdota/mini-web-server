//
// Created by sfcdo on 20.07.2021.
//

#ifndef WEBSERV_AREQUEST_HPP_
#define WEBSERV_AREQUEST_HPP_
#include "parser.hpp"
class ARequest {
 public:
  explicit ARequest(const ServerConfig & config);
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
  virtual ~ARequest();
	
protected:
	std::map<std::string, std::string> request_line;
	std::map<std::string, std::string> headers;
	std::string body;
  const ServerConfig & server_config;
  size_t status_code;
  bool closeOnEnd;
};
#endif //WEBSERV_AREQUEST_HPP_
