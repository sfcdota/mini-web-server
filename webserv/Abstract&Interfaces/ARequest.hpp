//
// Created by sfcdo on 20.07.2021.
//

#ifndef WEBSERV_AREQUEST_HPP_
#define WEBSERV_AREQUEST_HPP_
#include "parser.hpp"

class ARequest {
 public:
  ARequest(const ServerConfig & config);
  ARequest(const ARequest & in);
  ARequest & operator=(const ARequest & in);
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
