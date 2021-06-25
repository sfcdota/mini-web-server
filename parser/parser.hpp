#ifndef WEBSERV_PARSER_HPP_
#define WEBSERV_PARSER_HPP_

#include "server.hpp"
#define COMMENT '#'

int get_next_line(int fd, char **line);


/*server config functions*/
server config_par();
void server_func(conf_pars &con, int &i);
void location_func(conf_pars &con, int &i);


/*server config utils*/
void init_server(conf_pars &con);
void init_location(conf_pars &con);
void clear_loc(conf_pars &con);
void error_page(std::string str);
int to_int(std::string str);
void isstring(std::vector<std::string> &bla, std::string &tmp);
void word_spliter(char *line, std::vector<std::string> &bla);


#endif //WEBSERV_PARSER_HPP_
