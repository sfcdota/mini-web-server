//
// Created by sfcdo on 20.07.2021.
//

#include "ARequest.hpp"
ARequest::ARequest(const ServerConfig &config)
    : server_config(config), closeOnEnd(false) {}

ARequest::~ARequest() {}
