#include "CGI.hpp"


CGI::~CGI() {}

void CGI::executeCGI(Request & request, Response & response) {
  char **envp = setEnv(request, response);
  pid_t pid;
  int status;
  int fd[2], out = dup(STDOUT_FILENO), in = dup (STDIN_FILENO);
  pipe(fd);
  std::cout << "Forking CGI execution with target = " << request.request_line["target"] << std::endl;
  pid = fork();
  if (pid == -1) {
    throw std::runtime_error("error");
  } else if (pid == 0) {
    dup2(fd[0], STDIN_FILENO);
    dup2(fd[1], STDOUT_FILENO);
    char **kek = new char*[2];
    kek[0] = strdup((request.server_config.root + "/cgi/cgi_tester").c_str());
    kek[1] = NULL;
    std::cerr << "execve path = " << kek[0] << std::endl;
    if (execve(kek[0], kek, envp) == -1) {
//      throw std::runtime_error("Error: execve");
        std::cerr << errno << strerror(errno) << std::endl;
    }
    std::cerr << "END OF EXECVE CALL" << std::endl;
    exit(0);
  }
  std::cout << "START TO SEND BODY TO THE EXECVED CGI" << std::endl;
  const char *tmp_body = request.body.c_str();
//  dup2(fd[1], STDOUT_FILENO);
//  close(fd[1]);
  dup2(fd[0], STDIN_FILENO);
  close(fd[0]);
  for(int i = 0; i < request.body.length(); i += PIPE_BUF)
  write(fd[1], request.body.c_str(), request.body.length());
      close(fd[1]);
//  std::cout << request.body << std::endl;
//  dup2(out, STDOUT_FILENO);
  std::cout << "WAITING FOR EXECVE PROCESS" << std::endl;
  waitpid(pid, &status, 0);
  std::getline(std::cin, response.body, '\0');
  std::cout << "END OF GETTING CGI RESPONSE" << std::endl;
  dup2(in, STDIN_FILENO);
  deleteENVP(envp);
}

char ** CGI::mapToCString(std::map<std::string, std::string> &tmpEnv) {
  char **envp = reinterpret_cast<char **>(calloc(tmpEnv.size() + 1, sizeof(char *)));
  std::cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% ENVPS OF CGI %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
  std::map<std::string, std::string>::iterator it = tmpEnv.begin();
  for (int index = 0; it != tmpEnv.end(); ++it, ++index) {
    envp[index] = strdup((it->first + it->second).c_str());
    std::cout << envp[index] << std::endl;
  }
  std::cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% END OF ENVPS OF CGI %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
  return envp;
}

bool iishex(int c) {
  return (c > 47 && c < 58) || (c > 64 && c < 71) || (c > 94 && c < 103); // not sure about c > 94 && c < 103
}


bool iispctencoded(const std::string & s, size_t & index) {
  return s[index] == '%' && iishex(s[index + 1]) && iishex(s[index + 2]);
}


std::string & CGI::translate_path(std::string & path) {
  char c;
  for(size_t i = 0; i < path.length(); i++) {
    if (iispctencoded(path, i)) {
      path.replace(i, 3, &(c = static_cast<const char>(strtol(&path.c_str()[i + 1], NULL, 16))));
      i += 2;
    }
  }
  return path;
}

char ** CGI::setEnv(Request &req, Response & response) {
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
  tmpEnv["PATH_INFO="] = response.location_.root.substr(1) + (pos == it->second.size() ? it->second : it->second.substr(0, pos));
  std::string translated = req.server_config.root + response.location_.root + req.request_line["target"];
  tmpEnv["PATH_TRANSLATED="] = translate_path(translated);
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
  tmpEnv["SERVER_PORT="] = std::to_string(req.server_config.port);
  tmpEnv["SERVER_PROTOCOL="] = req.request_line["version"];
  tmpEnv["SERVER_SOFTWARE="] = "kekers228/v4.20";
//	tmpEnv["HTTP_X_SECRET_HEADER_FOR_TEST="] = "1";
  return mapToCString(tmpEnv);
}

void CGI::deleteENVP(char **envp) {
  if (envp) {
    for (int i = 0; envp[i] != NULL; ++i) {
      delete[] envp[i];
    }
    delete[] envp;
    envp = NULL;
  }
}
