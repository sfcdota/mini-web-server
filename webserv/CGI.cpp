#include "CGI.hpp"

CGI::CGI(const Request &req, const ServerConfig &con, std::string str)
		 : _env(NULL), request_(req), _con(con), str(str) {
	setEnv();
	executeCGI();
}

CGI::~CGI() {
	if (_env) {
		for (int i = 0; _env[i] != NULL; ++i) {
			delete[] _env[i];
		}
		delete[] _env;
		_env = NULL;
	}
}

void CGI::executeCGI() {
//	int oldFdIn = dup(0);
//	int oldFdOut = dup(1);
	int pid;

	int fd = open("cgiOut.txt", O_CREAT | O_RDWR | O_TRUNC);
	if (fd == -1) {
		std::cout << "knfonda" << std::endl;
	}
	pid = fork();
	if (pid == -1) {
		throw std::runtime_error("error");
	} else if (pid == 0) {
		char * const * kek= NULL;
		dup2(fd, STDOUT_FILENO);

		if (execve((_con.root + "/cgi/cgi_tester").c_str(), kek, _env) == -1) {
			throw std::runtime_error("Error: execve");
		}
	}
	wait(NULL);
	close(fd);
}

void CGI::mapToCString(std::map<std::string, std::string> &tmpEnv) {
	_env = reinterpret_cast<char **>(calloc(tmpEnv.size() + 1, sizeof(char *)));
	std::map<std::string, std::string>::iterator it = tmpEnv.begin();
	for (int index = 0; it != tmpEnv.end(); ++it, ++index) {
		_env[index] = strdup((it->first + it->second).c_str());
		std::cout << _env[index] << std::endl;
	}
}

void CGI::setEnv() {
//	AUTH_TYPE=
//	CONTENT_LENGTH=100000000
//	CONTENT_TYPE=test/file
//	GATEWAY_INTERFACE=CGI/1.1
//	PATH_INFO=/directory/youpi.bla
//	PATH_TRANSLATED=YoupiBanane/directory/youpi.bla
//	QUERY_STRING=
//	REDIRECT_STATUS=200
//	REMOTE_ADDR=127.0.0.1
//	REMOTE_IDENT=
//	REMOTE_USER=
//	REQUEST_METHOD=POST
//	REQUEST_URI=/directory/youpi.bla?
//			SCRIPT_FILENAME=test_linux/cgi_tester
//			SCRIPT_NAME=test_linux/cgi_tester
//	SERVER_NAME=youpi
//	SERVER_PORT=8070
//	SERVER_PROTOCOL=HTTP/1.1
//	SERVER_SOFTWARE=Weebserv/1.0



	std::map<std::string, std::string> tmpEnv;
	tmpEnv["AUTH_TYPE="] = "basic";
	tmpEnv["CONTENT_LENGTH="] = request_.headers.find("Content-Length")->second;//_headers.find("content-length")->second;
	tmpEnv["CONTENT_TYPE="] = request_.headers.find("Content-Type")->second;
	tmpEnv["GATEWAY_INTERFACE="] = "CGI/1.1";
	tmpEnv["PATH_INFO="] = "/directory/youpi.bla";
	tmpEnv["PATH_TRANSLATED="] = "YoupiBanane/directory/youpi.bla";
//			this->str;
	std::string tmpStr = this->request_.request_line.find("target")->second;
	tmpEnv["QUERY_STRING="] = tmpStr.erase(0, 1);
	tmpEnv["REDIRECT_STATUS"] = "200";
	tmpEnv["REMOTE_ADDR="] = "127.0.0.1";
	tmpEnv["REMOTE_IDENT="] = "basic";
	tmpEnv["REMOTE_USER="] = "?????";
	tmpEnv["REQUEST_METHOD="] = request_.request_line.find("method")->second;
	tmpEnv["REQUEST_URI="] = "/directory/youpi.bla";
	tmpEnv["SCRIPT_FILENAME"] = "cgi/cgi_tester";
	tmpEnv["SCRIPT_NAME="] = "cgi/cgi_tester";
//	if (_con.server_names.size()) {
//		tmpEnv["SERVER_NAME="] = _con.server_names[0];
//	} else {
	tmpEnv["SERVER_NAME="] = std::to_string(_con.host);
//	}
	tmpEnv["SERVER_PORT="] = std::to_string(_con.port);
	tmpEnv["SERVER_PROTOCOL="] = request_.request_line.find("version")->second;
	tmpEnv["SERVER_SOFTWARE="] = "kekers228/v4.20";
//	tmpEnv["HTTP_X_SECRET_HEADER_FOR_TEST="] = "1";
	mapToCString(tmpEnv);
	tmpEnv.clear();
}
