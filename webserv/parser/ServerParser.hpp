#ifndef WEBSERV_SERVERPARSER_HPP
#define WEBSERV_SERVERPARSER_HPP

#include "../includes/parser.hpp"

class ServerParser {
	ServerParser();
	
	~ServerParser();

public:
	static void ParsLocation(parsConfig &con, int &i);
	static void ParsServer(parsConfig &con, int &i);
	static ServerConfig	ParsConfs(parsConfig &con);
	static std::vector<ServerConfig> Run();
};


#endif //WEBSERV_SERVERPARSER_HPP
