#include "parser.hpp"

void split_words(std::string str, std::vector<std::string> &arr)
{
	std::string word = "";
	for (int i = 0; i < str.size(); i++)
		if (str[i] == ' ' || str[i] == '\t')
		{
			if (word.size())
				arr.push_back(word);
			word = "";
		}
		else
			word += str[i];
	if (word.size())
		arr.push_back(word);
}

void	init_parser(parser &pars){
	pars.server_status = false;
	pars.location_status = false;
}

int check_server(parser &pars, bool &command_status, const std::string &command){
	bool in_line = false;

	for(int i = 0; i < pars.string_arr.size(); i++){
		if (pars.string_arr[i][0] == COMMENT && (!in_line || command_status))
			break;
		if (pars.string_arr[i] == command && !command_status) {
			in_line = true;
			continue;
		}
		if (pars.string_arr[i] == "{" && in_line) {
			command_status = true;
			continue;
		}
		return 1;
	}
	return 0;
}

int to_int(std::string str){
	long n;
	n = 0;
	for(int i = 0; i < str.size(); i++) {
		if (n > 65535)
			return -1;
		if (str[i] >= '0' && str[i] <= '9')
			n = n * 10 + (str[i] - 48);
		else
			return -1;
	}
	return n;
}

int pars_listen(parser &pars, server &serv){
	int delimeter;

	if (pars.string_arr.size() >= 2) {
		if (pars.string_arr[1][0] == COMMENT) {
			pars.error_message = "Wrong config file:\n\t'listen' format error!\n";
			return 1;
		}
		else {
			if ((delimeter = pars.string_arr[1].find(":")) == -1) {
				pars.error_message = "Wrong config file:\n\tsome problem with url (listen) format!\n";
				return 1;
			}
			serv.host = pars.string_arr[1].substr(0, delimeter);
			serv.port= to_int(pars.string_arr[1].substr(delimeter + 1,
											   pars.string_arr.size() - delimeter));
			if (serv.port < 0) {
				pars.error_message = "Wrong config file:\n\tport error!\n";
				return 1;
			}
		}
		if (pars.string_arr.size() > 2 && pars.string_arr[2][0] != COMMENT) {
			pars.error_message = "Wrong config file:\n\t'listen' format error!\n";
			return 1;
		}
	}
	return 0;
}

int pars_error_page(parser &pars, server &serv){
	error_page er;

	if (pars.string_arr.size() > 2) {
		if (pars.string_arr[1][0] == COMMENT || pars.string_arr[2][0] == COMMENT) {
			pars.error_message = "Wrong config file:\n\t'error_page' format error!\n";
			return 1;
		}
		else {
			if ((er.error_code = to_int(pars.string_arr[1])) < 0){
				pars.error_message = "Wrong config file:\n\twrong 'error_page' code!\n";
				return 1;
			}
			er.error_path = pars.string_arr[2];
			serv.error_pages.push_back(er);
		}
		if (pars.string_arr.size() > 3 && pars.string_arr[3][0] != COMMENT) {
			pars.error_message = "Wrong config file:\n\t'error_page' format error!\n";
			return 1;
		}
	}
	return 0;
}

int pars_server_name(parser &pars, server &serv){
	if (pars.string_arr.size() >= 2) {
		if (pars.string_arr[1][0] == COMMENT) {
			pars.error_message = "Wrong config file:\n\t'error_page' format error!\n";
			return 1;
		}
		else {
			serv.server_names.push_back(pars.string_arr[1]);
		}
		if (pars.string_arr.size() > 2 && pars.string_arr[2][0] != COMMENT) {
			pars.error_message = "Wrong config file:\n\t'server_name' format error!\n";
			return 1;
		}
	}
	return 0;
}

int pars_client_max_body_size(parser &pars, server &serv){
	if (pars.string_arr.size() >= 2) {
		if (pars.string_arr[1][0] == COMMENT) {
			pars.error_message = "Wrong config file:\n\t'error_page' format error!\n";
			return 1;
		}
		else {
			if ((serv.client_max_body_size = to_int(pars.string_arr[1]) < 0)){
				pars.error_message = "Wrong config file:\n\t'client_max_body_size' error!\n";
				return 1;
			}
		}
		if (pars.string_arr.size() > 2 && pars.string_arr[2][0] != COMMENT) {
			pars.error_message = "Wrong config file:\n\t'client_max_body_size' format error!\n";
			return 1;
		}
	}
	return 0;
}

int check_location(parser &pars, server &serv, location &loc){
	if (pars.string_arr.size() >= 3) {
		if (pars.string_arr[1][0] == COMMENT || pars.string_arr[2][0] == COMMENT) {
			pars.error_message = "Wrong config file:\n\t'location' format error!\n";
			return 1;
		}
		else {
			loc.path = pars.string_arr[1];
			if (pars.string_arr[2] == "{")
				pars.location_status = true;
			else{
				pars.error_message = "Wrong config file:\n\t'wrong location body!\n";
				return 1;
			}
		}
		if (pars.string_arr.size() > 3 && pars.string_arr[3][0] != COMMENT) {
			pars.error_message = "Wrong config file:\n\t'location' format error!\n";
			return 1;
		}
	}
	return 0;
}

int deal_with_data(parser &pars, server &serv, location &loc){

	if (pars.string_arr[0][0] == COMMENT)
		return 0;
	if (pars.string_arr[0] == "listen" && pars_listen(pars, serv))
			return 1;
	if (pars.string_arr[0] == "server_name" && pars_server_name(pars, serv))
			return 1;
	if (pars.string_arr[0] == "error_page" && pars_error_page(pars, serv))
			return 1;
	if (pars.string_arr[0] == "client_max_body_size" && pars_client_max_body_size(pars, serv))
		return 1;
	if ((pars.string_arr[0] == "location" || pars.location_status) && check_location(pars, serv, loc))
		return 1;



	return 0;
}

int main() {
	parser		pars;
	server		serv;
	location	loc;

	init_parser(pars);
//	init_serv(serv);
	pars.fd = open("./server.conf", O_RDONLY);

	while ((pars.res = get_next_line(pars.fd, pars.str)) > -1) {
//		init_loc(loc);
		split_words(pars.str, pars.string_arr);
		if (pars.string_arr.size())
		{
			if (!pars.server_status)
				if (check_server(pars, pars.server_status, "server")) {
					std::cout << "Wrong config file:\n\twrong server body\n";
					exit(1);
				}
			if (deal_with_data(pars, serv, loc)) {
				std::cout << pars.error_message;
				exit(1);
			}
		}
		pars.string_arr.clear();
		pars.str = "";
		if (!pars.res)
			break;
	}
	return 0;
}