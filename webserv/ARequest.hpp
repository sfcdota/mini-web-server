//
// Created by sfcdo on 20.07.2021.
//

#ifndef WEBSERV_AREQUEST_HPP_
#define WEBSERV_AREQUEST_HPP_
#include "includes/parser.hpp"
class ARequest {
 public:
  explicit ARequest(const ServerConfig & config);
  virtual const std::map<std::string, std::string> &GetRequestLine() const = 0;
  virtual const std::map<std::string, std::string> &GetHeaders() const = 0;
  virtual const std::string & GetBody() const = 0;
  virtual void SetBody(const std::string & body) const = 0;
  virtual ~ARequest();
 private:
  std::map<std::string, std::string> request_line,
                                     headers;
  std::string body;
  const ServerConfig & server_config;
};
#endif //WEBSERV_AREQUEST_HPP_
