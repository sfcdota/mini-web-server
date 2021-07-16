#include "CGI.hpp"

CGI::CGI(Request &req, const ServerConfig &con, std::string & str, std::string & result_dst)
    : _env(NULL), _con(con), str(str), request_(req) {
  setEnv(req);
  executeCGI(result_dst);
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

void CGI::executeCGI(std::string & result_dst) {
//	int oldFdIn = dup(0);
//	int oldFdOut = dup(1);
  pid_t pid;
  int status;

  pid = fork();
  if (pid == -1) {
    throw std::runtime_error("error");
  } else if (pid == 0) {
    std::string hui = _con.root + "/cgi/cgi_tester";
    char* kek = const_cast<char *>(hui.c_str());
    dup2(STDIN_FILENO, STDOUT_FILENO);
    if (execve(kek, reinterpret_cast<char *const *>(&kek), _env) == -1) {
      throw std::runtime_error("Error: execve");
    }
    exit(0);
  }
  waitpid(pid, &status, 0);
  std::cin >> result_dst;
}

void CGI::mapToCString(std::map<std::string, std::string> &tmpEnv) {
  _env = reinterpret_cast<char **>(calloc(tmpEnv.size() + 1, sizeof(char *)));
  std::map<std::string, std::string>::iterator it = tmpEnv.begin();
  for (int index = 0; it != tmpEnv.end(); ++it, ++index) {
    _env[index] = strdup((it->first + it->second).c_str());
    std::cout << _env[index] << std::endl;
  }
}

std::string & CGI::translate_path(std::string & path) {
  char c;
//  for(size_t i = 0; i < path.length(); i++) {
//    if (request_.validator_.ispctencoded(path, i)) {
//      path.replace(i, 3, &(c = static_cast<const char>(strtol(&path.c_str()[i + 1], NULL, 16))));
//      i += 2;
//    }
//  }
  return path;
}

void CGI::setEnv(Request &req) {
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
  std::map<std::string, std::string>::iterator it;
  std::map<std::string, std::string>::iterator end = tmpEnv.end();
  size_t pos;

//  if(req.headers.find("WWW-Authenticate") != req.headers.end())
//    tmpEnv["AUTH_TYPE="] = "Basic";
  tmpEnv["CONTENT_LENGTH="] = req.headers["Content-Length"];
  it = req.headers.find("Content-Type");
  if (it != end)
    tmpEnv["CONTENT_TYPE="] = it->second;
  tmpEnv["GATEWAY_INTERFACE="] = "CGI/1.1";
  it = req.request_line.find("target");
  pos = it->second.find('?');
  tmpEnv["PATH_INFO="] = pos == it->second.size() ? it->second : it->second.substr(0, pos);
  tmpEnv["PATH_TRANSLATED="] = translate_path(str);
  tmpEnv["QUERY_STRING="] = pos + 1 > it->second.length() ? it->second.substr(pos + 1) : "";
//  tmpEnv["REDIRECT_STATUS"] = "200";
  tmpEnv["REMOTE_ADDR="] = inet_ntoa(req.addr.sin_addr) ;
//  tmpEnv["REMOTE_IDENT="] = "basic";
//  tmpEnv["REMOTE_USER="] = "?????";
  tmpEnv["REQUEST_METHOD="] = req.request_line["method"];
  tmpEnv["REQUEST_URI="] = req.request_line["target"];
  tmpEnv["SCRIPT_NAME="] = "cgi/cgi_tester";
  tmpEnv["SCRIPT_FILENAME="] = req.server_config.root + "/cgi/cgi_tester";

//todo rework with inet_ntoa
//  tmpEnv["SERVER_NAME="] = std::to_string(_con.host);
  tmpEnv["SERVER_NAME="] = "127.0.0.1";
  tmpEnv["SERVER_PORT="] = std::to_string(_con.port);
  tmpEnv["SERVER_PROTOCOL="] = req.request_line["version"];
  tmpEnv["SERVER_SOFTWARE="] = "kekers228/v4.20";
//	tmpEnv["HTTP_X_SECRET_HEADER_FOR_TEST="] = "1";
  mapToCString(tmpEnv);
  tmpEnv.clear();
}
