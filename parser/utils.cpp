#include "parser.hpp"

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

void error_page(std::string str){
	std::cout << str << std::endl;
	exit(1);
}

void clear_loc(conf_pars &con){
	con.loc.autoindex = false;
	con.loc.upload_path = "";
	con.loc.http_methods.clear();
	con.loc.cgi_extension = "";
	con.loc.cgi_path = "";
	con.loc.index.clear();
	con.loc.root = "";
	con.loc.retur.clear();
}

void init_location(conf_pars &con){
	con.location_body["autoindex"] = 0;
	con.location_body["limit_except"] = 0;
	con.location_body["root"] = 0;
	con.location_body["index"] = 0;
	con.location_body["cgi_extension"] = 0;
	con.location_body["cgi_path"] = 0;
	con.location_body["upload_path"] = 0;
	con.location_body["return"] = 0;
	con.pars.location_status = 0;
}

void init_server(conf_pars &con){
	con.server_body["listen"] = 0;
	con.server_body["server_name"] = 0;
	con.server_body["error_page"] = 0;
	con.server_body["client_max_body_size"] = 0;
	con.server_body["return"] = 0;
	con.pars.server_status = 0;
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
		if (*line == COMMENT) {
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