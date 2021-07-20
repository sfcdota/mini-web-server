//
// Created by sfcdo on 20.07.2021.
//

#ifndef WEBSERV_ASERVER_HPP_
#define WEBSERV_ASERVER_HPP_
#include <vector>
#include "includes/parser.hpp"
#include "BufferProcessor.hpp"
class AServer {
 protected:
  AServer(const std::vector<ServerConfig>& config, const ssize_t & INPUT_BUFFER_SIZE);
  virtual ~AServer();
  fd_set master_read_, working_read_,
         master_write_, working_write_;
  timeval timeout_;
  const std::vector<ServerConfig>& config_;
  BufferProcessor buffer_processor_;
};

#endif //WEBSERV_ASERVER_HPP_
