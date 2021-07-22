#ifndef WEBSERV_PARSER_HPP_
#define WEBSERV_PARSER_HPP_
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
struct error {
	int error_code;
	std::string error_path;
};

struct location {
	std::string path;
	bool autoindex;
	std::string root;
	size_t max_body;
	std::vector<std::string> index;
	std::vector<std::string> http_methods;
	std::string upload_path; // if exists - upload allowed
	std::string cgi_extension;
	std::string cgi_path;
	std::vector<std::string> retur;
    std::vector<error> error_pages;
};

struct ServerConfig {
	int host;
	int port;
	std::string root;
	std::vector<std::string> server_names;
	std::vector<error> error_pages;
	long long client_max_body_size;
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

struct parsConfig {
	parser pars;
	ServerConfig serv;
	location location;
	std::map <std::string, short> location_body;
	std::map <std::string, short> server_body;
	std::vector<std::string> vec;
};
//#include "server.hpp"
#define COMMENT '#'

int get_next_line(int fd, char **line);


/*server config_ functions*/
std::vector<ServerConfig> parsConf();
void ParsServer(parsConfig &con, int &i);
void ParsLocation(parsConfig &con, int &i);


/*server config_ utils*/
void init_server(parsConfig &con);
void init_location(parsConfig &con);
void clear_loc(parsConfig &con);
void errors(std::string str);
size_t to_int(std::string str);
void isstring(std::vector<std::string> &bla, std::string &tmp);
void word_spliter(char *line, std::vector<std::string> &bla);
std::string rootDir();
bool SearchForDir(const std::string &path, int i);
bool SearchForFile(const std::string &path);
void createHTMLAutoIndex(ServerConfig &con, std::string linkPath);

#endif //WEBSERV_PARSER_HPP_
