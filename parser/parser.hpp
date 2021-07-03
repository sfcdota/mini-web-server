#ifndef WEBSERV_PARSER_HPP_
#define WEBSERV_PARSER_HPP_
#include "../webserv/includes/allowed_library_includes.hpp"

struct error {
	int error_code;
	std::string error_path;
};

struct loc {
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

struct conf {
	std::string host;
	int port;
	std::string root;
	std::vector<std::string> server_names;
	std::vector<error> error_pages;
	int client_max_body_size;
	std::vector<std::string> retur;
	std::vector<loc> locations;
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
	conf serv;
	loc location;
	std::map <std::string, short> location_body;
	std::map <std::string, short> server_body;
	std::vector<std::string> vec;
};
//#include "server.hpp"
#define COMMENT '#'

int get_next_line(int fd, char **line);


/*server config functions*/
conf parsConf();
void parsServer(parsConfig &con, int &i);
void parsLocation(parsConfig &con, int &i);


/*server config utils*/
void init_server(parsConfig &con);
void init_location(parsConfig &con);
void clear_loc(parsConfig &con);
void errors(std::string str);
int to_int(std::string str);
void isstring(std::vector<std::string> &bla, std::string &tmp);
void word_spliter(char *line, std::vector<std::string> &bla);
std::string rootDir();
bool SearchForDir(const std::string &path);
bool SearchForFile(const std::string &path);


#endif //WEBSERV_PARSER_HPP_
