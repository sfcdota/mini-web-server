//
// Created by sfcdo on 20.07.2021.
//

#include "ARequest.hpp"
ARequest::ARequest(const ServerConfig &config)
    : server_config(config), closeOnEnd(false) {}
ARequest::ARequest(const ARequest &in): server_config(in.server_config) {
  *this = in;
}
ARequest &ARequest::operator=(const ARequest &in) {
  request_line = in.request_line;
  headers = in.headers;
  body = in.body;
  status_code = in.status_code;
  closeOnEnd = in.closeOnEnd;
  return *this;
}

ARequest::~ARequest() {}
