//
// Created by sfcdo on 20.07.2021.
//

#ifndef WEBSERV_ARESPONSE_HPP_
#define WEBSERV_ARESPONSE_HPP_
#include "../Request.hpp"
class AResponse {
 protected:
  explicit AResponse(const Request & request);
  virtual ~AResponse();
  virtual const std::string & GetResponse() const = 0;
  std::string response;
  const Request & request;
};
#endif //WEBSERV_ARESPONSE_HPP_
