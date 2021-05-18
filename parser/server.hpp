#ifndef SERVER_HPP_
#define SERVER_HPP_
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <vector>

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

struct server {
  int host;
  int port;
  std::vector<std::string> server_names;
  std::vector<error_page> error_pages;
  size_t client_max_body_size;
  std::vector<location> locations;
};
#endif // SERVER_HPP_
