#include "CGI.hpp"

CGI::CGI(const std::map<std::string, std::string> &request_line, const ServerConfig &con,
		 const std::map<std::string, std::string> &headers)
		 : _env(NULL), _headers(headers), _request_line(request_line), _con(con)
{
	setEnv();
}

CGI::~CGI()
{
	if (_env) {
		for (int i = 0; _env[i] != NULL; ++i) {
			delete[] _env[i];
		}
		delete[] _env;
		_env = NULL;
	}
}

void CGI::executeCGI() {
	int pipes[2];
	int oldFdIn = dup(0);
	int oldFdOut = dup(1);
	int pid;

	pipe(pipes);
	pid = fork();
	if (pid == -1) {
		throw std::runtime_error("error");
	} if (pid == 0) {
//		execve();
	}
	wait(NULL);
	close(oldFdIn);
	close(oldFdOut);
}

void CGI::mapToCString(std::map<std::string, std::string> &tmpEnv) {
	_env = (char **)calloc(tmpEnv.size() + 1, sizeof(char *));
	std::map<std::string, std::string>::iterator it = tmpEnv.begin();
	for (int index = 0; it != tmpEnv.end(); ++it, ++index) {
		_env[index] = strdup((it->first + it->second).c_str());
		std::cout << _env[index] << std::endl;
	}
}

void CGI::setEnv() {
	std::map<std::string, std::string> tmpEnv;

	tmpEnv["AUTH_TYPE="] = "basic";
	tmpEnv["CONTENT_LENGTH="] = _headers.find("content-length")->second;
	tmpEnv["CONTENT_TYPE="] = "text/html";
	tmpEnv["GATEWAY_INTERFACE="] = "CGI/1.1";
	tmpEnv["PATH_INFO="] = "?????";
	tmpEnv["PATH_TRANSLATED="] = "?????";
	std::string tmpStr = _request_line.find("target")->second;
	tmpEnv["QUERY_STRING="] = tmpStr.erase(0, 2);
	tmpEnv["REMOTE_ADDR="] = "?????";
	tmpEnv["REMOTE_IDENT="] = "basic";
	tmpEnv["REMOTE_USER="] = "?????";
	tmpEnv["REQUEST_METHOD="] = _request_line.find("method")->second;
	tmpEnv["REQUEST_URI="] = "?????";
	tmpEnv["SCRIPT_NAME="] = "?????";
	if (_con.server_names.size()) {
		tmpEnv["SERVER_NAME="] = _con.server_names[0];
	} else {
		tmpEnv["SERVER_NAME="] = std::to_string(_con.host);
	}
	tmpEnv["SERVER_PORT="] = std::to_string(_con.port);
	tmpEnv["SERVER_PROTOCOL="] = _request_line.find("version")->second;
	tmpEnv["SERVER_SOFTWARE="] = "kekers228/v4.20";
	tmpEnv["HTTP_X_SECRET_HEADER_FOR_TEST="] = "1";
	mapToCString(tmpEnv);
	tmpEnv.clear();
};
