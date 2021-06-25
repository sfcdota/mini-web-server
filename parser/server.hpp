#ifndef SERVER_HPP_
#define SERVER_HPP_
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

struct error_page {
  int error_code;
  std::string error_path;
};

struct location {
  std::string path;
  bool autoindex;
  std::string root;
  std::vector<std::string> index;
  std::vector<std::string> http_methods;
  std::string upload_path; // if exists - upload allowed
  std::string cgi_extension;
  std::string cgi_path;
  std::vector<std::string> retur;
};

struct server {
  std::string host;
  int port;
  std::vector<std::string> server_names;
  std::vector<error_page> error_pages;
  int client_max_body_size;
  std::vector<std::string> retur;
  std::vector<location> locations;
};

struct parser {
  int fd;
  int find;
  std::string str;
  int index;
  short server_status;
  short location_status;
  std::vector<std::string> string_arr;
  int res;
  std::string error_message;
};

struct conf_pars {
	parser pars;
	server serv;
	location loc;
	std::map <std::string, short> location_body;
	std::map <std::string, short> server_body;
	std::vector<std::string> vec;
};
#endif // SERVER_HPP_
