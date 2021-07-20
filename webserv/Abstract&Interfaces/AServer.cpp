//
// Created by sfcdo on 20.07.2021.
//

#include "AServer.hpp"
AServer::AServer(const std::vector<ServerConfig> &configs, const ssize_t & INPUT_BUFFER_SIZE)
    : master_read_(), working_read_(), master_write_(), working_write_(),
      timeout_(), config_(configs), buffer_reader_(INPUT_BUFFER_SIZE) {
  FD_ZERO(&master_write_);
  FD_ZERO(&working_write_);
  FD_ZERO(&master_read_);
  FD_ZERO(&working_read_);
}
AServer::~AServer() {}
