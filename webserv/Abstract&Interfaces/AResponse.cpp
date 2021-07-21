//
// Created by sfcdo on 20.07.2021.
//

#include "AResponse.hpp"
AResponse::AResponse(const Request &request)
  : _request(request), _serverConf(request.GetServerConfig()) {}

AResponse::~AResponse() {

}
