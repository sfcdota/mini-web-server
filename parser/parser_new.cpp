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

void location_func(std::vector<std::string> vec, location &loc, std::map<std::string, short> location_body, parser & pars, int &i){
  std::string tmp;
	for (; i < vec.size(); i++){
	  if (vec[i] == "root"){
		while (vec[++i] != ";");
	  }
	  else if (vec[i] == "autoindex" && !location_body["autoindex"]){
	    location_body["autoindex"] = 1;
		if (vec[i + 1] == "on")
		  loc.autoindex = true;
		else if (vec[i + 1] == "off")
		  loc.autoindex = false;
		else
		  error_page("location autoindex error!");
		if (vec[i + 2] != ";")
		  error_page("location autoindex error!");
		i += 2;
	  }
	  else if (vec[i] == "index" && !location_body["index"]){
		while (++i < vec.size(), vec[i] != ";"){
		  loc.index.push_back(vec[i]);
		  if (!location_body["index"])
		  location_body["index"] = 1;
		}
		if (i == vec.size() || !location_body["index"])
		  error_page("location index error!");
	  }
	  else if (vec[i] == "limit_except" && !location_body["limit_except"]){
		while (++i < vec.size(), vec[i] != ";"){
		  if (vec[i] == "POST" || vec[i] == "PUT" || vec[i] == "DELETE" || vec[i] == "GET")
			  loc.index.push_back(vec[i]);
		  else
			error_page("locaiton limit_except error!");
		  if (!location_body["index"])
			location_body["index"] = 1;
		}
		if (i == vec.size() || !location_body["index"])
		  error_page("location index error!");
	  }
	  else if (vec[i] == "upload_path" && !location_body["upload_path"]){
		location_body["upload_path"] = 1;
		if (vec[i + 1] != ";")
		  loc.upload_path = vec[i + 1];
		else
		  error_page("location upload_path error!");
		if (vec[i + 2] != ";")
		  error_page("location upload_path error!");
		i += 2;
	  }
	  else if (vec[i] == "cgi_extension" && !location_body["cgi_extension"]){
		location_body["cgi_extension"] = 1;
		if (vec[i + 1] != ";")
		  loc.cgi_extension = vec[i + 1];
		else
		  error_page("location cgi_extension error!");
		if (vec[i + 2] != ";")
		  error_page("location cgi_extension error!");
		i += 2;
	  }
	  else if (vec[i] == "cgi_path" && !location_body["cgi_path"]){
		location_body["cgi_path"] = 1;
		if (vec[i + 1] != ";")
		  loc.upload_path = vec[i + 1];
		else
		  error_page("location cgi_path error!");
		if (vec[i + 2] != ";")
		  error_page("location cg_path error!");
		i += 2;
	  }
	  else if (vec[i] == "}")
	    pars.location_status = false;
	  else
		error_page("location error!");
	}
}

void server_func(location &loc, std::vector<std::string> vec, server &serv, std::map<std::string, short> server_body,std::map<std::string, short> location_body, parser &pars, int &i){
	std::string tmp;

  for(; i < vec.size(); i++){
	if (vec[i] == "location")
	  pars.location_status = 1;
	else if (vec[i] != "{" && pars.location_status == 1) {
	  loc.path = vec[i];
	  pars.location_status = 2;
	}
	else if (vec[i] == "{" && pars.location_status == 2)
	  pars.location_status = 3;
	else if (pars.location_status == 3)
	  location_func(vec, loc, location_body, pars, i);
	else if (vec[i] == "listen" && !server_body["listen"]){
	  server_body["listen"] = 1;
	  while (++i < vec.size() && vec[i] != ";"){
		int delimeter = vec[i].find(':');
		if (delimeter == -1)
		  error_page("server listen error!");
		serv.host = vec[i].substr(0, delimeter);
		serv.port = to_int(vec[i].substr(delimeter + 1, vec[i].size() - 1 - delimeter));
		if (serv.port == -1)
		  error_page("server listen port error!");
	  }
		if (i == vec.size())
			error_page("server listen error!");
	}
	else if (vec[i] == "server_name" && !server_body["server_name"]){
	  server_body["server_name"] = 1;
	  if (vec[i + 1] != ";")
	    serv.server_names.push_back(vec[i + 1]);
	  if (vec[i + 2] != ";")
			error_page("server: server_name error!");
	  i += 2;
	}
	else if (vec[i] == "error_page" && !server_body["error_page"]){
	  struct error_page err;
	  server_body["error_page"] = 1;
	  if (++i < vec.size() && vec[i] != ";")
		  err.error_code = to_int(vec[i++]);
	  if (err.error_code < 400 || err.error_code >= 500)
		error_page("server wrong error code!");
	  if (i < vec.size() && vec[i] != ";")
		err.error_path = vec[i++];
	  else
		error_page("server error_page error!");
	  if (i == vec.size() || vec[i] != ";")
		error_page("server error_page error!");
	  serv.error_pages.push_back(err);
	}
	else if (vec[i] == "client_max_body_size" && !server_body["client_max_body_size"]){
	  server_body["client_max_body_size"] = 1;
	  if (++i < vec.size())
		serv.client_max_body_size = to_int(vec[i++]);
	  if (serv.client_max_body_size == -1)
		  error_page("server client_max_body_size error!");
	  if (i == vec.size() || vec[i] != ";")
		error_page("server client_max_body_size error!");
	}
	else if (vec[i] == "}")
	    pars.server_status = false;
	else
	  error_page("server body error!");
  }
}
void isstring(std::vector<std::string> &bla, std::string &tmp){
  if (tmp.size()){
	bla.push_back(tmp);
	tmp = "";
  }
}
void word_spliter(char *line, std::vector<std::string> &bla){
  std::string tmp;

  while (*line){
    if (isspace(*line)) {
	  isstring(bla, tmp);
      line++;
	  continue;
	}
    if (*line == '#') {
	  isstring(bla, tmp);
	  break;
	}
	if (*line == '{' || *line == '}' || *line == ';'){
	  isstring(bla, tmp);
	  tmp.push_back(*line);
	  bla.push_back(tmp);
	  tmp = "";
	  line++;
	  continue;
	}
    tmp += *line;
    line++;
  }
  isstring(bla, tmp);
}

int main() {
  parser pars;
  server serv;
  location loc;
  std::map <std::string, short> location_body;
  location_body["autoindex"] = 0;
  location_body["limit_except"] = 0;
  location_body["root"] = 0;
  location_body["index"] = 0;
  location_body["cgi_extension"] = 0;
  location_body["cgi_path"] = 0;
  location_body["upload_path"] = 0;
  std::map <std::string, short> server_body;
  server_body["listen"] = 0;
  server_body["server_name"] = 0;
  server_body["error_page"] = 0;
  server_body["client_max_body_size"] = 0;
  char *line;
  std::string tmp;
  std::vector<std::string> vec;
  pars.server_status = 0;
  pars.location_status = 0;
  pars.fd = open("./server.conf", O_RDONLY);
  while ((pars.res = get_next_line(pars.fd, &line)) > -1) {
	word_spliter(line, vec);
	for (int i = 0; i < vec.size(); i++)
	  if (vec[i] == "server")
			pars.server_status = 1;
	  else if (vec[i] == "{" && pars.server_status == 1)
		  pars.server_status = 2;
	  else if (pars.server_status == 2)
		server_func(loc, vec, serv, server_body, location_body, pars, i);
	  else
		  error_page("server body error");
	vec.clear();
	if (!pars.res)
	  break;
  }
  return 0;
}