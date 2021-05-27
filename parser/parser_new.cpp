#include "parser.hpp"

void error_page(std::string str){
	std::cout << str << std::endl;
	exit(1);
}

char *whitespace(char *ch){
  while(isspace(*ch) && ++ch);
  return ch;
}
std::string split_to_word(char *ch){
	std::string str;
	while (*ch && !isspace(*ch) && *ch != ';')
		str.push_back(*(ch++));
	return str;
}

int server_status(std::map<std::string, int> &serv, std::string str){
  std::map<std::string, int>::iterator begin = serv.begin();
  std::map<std::string, int>::iterator end = serv.end();
  while (begin != end) {
	  if (str == begin->first && !begin->second) {
		  begin->second = 1;
		  return 0;
	  }
	  begin++;
  }
  return 1;
}
int to_int(std::string str){
	int n = 0;
	for (int i = 0; i < str.size(); i++) {
		if (n > 65535)
			return -1;
		if (str[i] >= '0' && str[i] <= '9')
			n = n * 10 + (str[i] - 48);
		else
			return -1;
	}
	return n;
}

void location_func(char *line, server &serv, ){

}

void server_func(char *line, server &serv, std::map<std::string, int> server_body, parser &pars){
	std::string tmp;
  while(*line){
  	line = whitespace(line);
  	if (*line == '#')
  		break;
	  line = whitespace(line);
	  if (split_to_word(line) == "location"){
		  if (tmp == "server") {
			  line += 6;
			  line = whitespace(line);
			  pars.server_status = true;
			  if (*line == '{')
				  line++;
			  else
				  error_page("server body error");
		  }
		  location_func(line, serv, server_body, pars);
	  }
	  else if (split_to_word(line) == "listen" && !server_body["listen"]){
	  	server_body["listen"] = 1;
	  	line += 6;
		  line = whitespace(line);
		  tmp = split_to_word(line);
		  line += tmp.size();
		  line = whitespace(line);
		  if (*line != ';')
			  error_page("server listen error!");
		int delimeter = tmp.find(':');
		if (delimeter == -1)
			error_page("server listen error!");
		serv.host = tmp.substr(0, delimeter);
		serv.port = to_int(tmp.substr(delimeter + 1, tmp.size() - 1 - delimeter));
		if (serv.port == -1)
			error_page("server listen port error!");
		line++;
	  }
	  else if (split_to_word(line) == "server_name" && !server_body["server_name"]){
	  	server_body["server_name"] = 1;
	  	line += 11;
	  	line = whitespace(line);
		  tmp = split_to_word(line);
		  line += tmp.size();
		  line = whitespace(line);
		  if (*line != ';')
			  error_page("server server_name error!");
		  serv.server_names.push_back(tmp);
		  line++;
	  }
	  else if (split_to_word(line) == "error_page" && !server_body["error_page"]){
	  	struct error_page err;
	  	server_body["error_page"] = 1;
	  	line += 10;
		  line = whitespace(line);
		  tmp = split_to_word(line);
		  line += tmp.size();
		  line = whitespace(line);
		  err.error_code = to_int(tmp);
		  if (err.error_code < 400 || err.error_code >= 500)
			  error_page("server wrong error code!");
		  tmp = split_to_word(line);
		  err.error_path = tmp;
		  line += tmp.size();
		  line = whitespace(line);
		  if (*line != ';')
			  error_page("server error_page error!");
		  serv.error_pages.push_back(err);
		  line++;
	  }
	  else if (split_to_word(line) == "client_max_body_size" && !server_body["client_max_body_size"]){
		server_body["client_max_body_size"] = 1;
	  	line += 20;
		  line = whitespace(line);
		  tmp = split_to_word(line);
		  line += tmp.size();
		  line = whitespace(line);
		  serv.client_max_body_size = to_int(tmp);
		  if (serv.client_max_body_size == -1)
			  error_page("server body_size error!");
		  if (*line != ';')
			  error_page("server server_name error!");
		  line++;
	  }
	  else
	  	error_page("server body error!");
  }
}



int main() {
  parser pars;
  server serv;
  location loc;
  std::map <std::string, int> server_body;
  server_body["listen"] = 0;
  server_body["server_name"] = 0;
  server_body["error_page"] = 0;
  server_body["client_max_body_size"] = 0;
  int server_b[6] = {0};
  char *line;
  std::string tmp;
  pars.fd = open("../server.conf", O_RDONLY);
  while ((pars.res = get_next_line(pars.fd, &line)) > -1) {
  	line = whitespace(line);
	if (*line == '#')
		continue;
	tmp = split_to_word(line);
	if (tmp == "server" || pars.server_status){
	  if (tmp == "server") {
		  line += 6;
		  line = whitespace(line);
		  pars.server_status = true;
		  if (*line == '{')
		  	line++;
		  else
			  error_page("server body error");
	  }
	  server_func(line, serv, server_body, pars);

    }
	else
		error_page("server body error");
	if (!pars.res)
	  break;
  }
  return 0;
}