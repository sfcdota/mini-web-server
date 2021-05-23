#ifndef WEBSERV_SERVERCONFIG_HPP_
#define WEBSERV_SERVERCONFIG_HPP_
#include "allowed_library_includes.hpp"
#include <sys/errno.h>
struct error_page {
  std::vector<int> error_codes;
  std::string error_path;
};

struct location {
  std::string path;
  bool autoindex;
  std::vector<std::string> index;
  std::vector<std::string> http_methods;
  std::string upload_path; // if exists - upload allowed
  std::string cgi_extension;
  std::string cgi_path;
};

struct ServerConfig {
  int host;
  int port;
  std::vector<std::string> server_names;
  std::vector<error_page> error_pages;
  size_t client_max_body_size;
  std::vector<location> locations;
};


#endif // WEBSERV_SERVERCONFIG_HPP_
