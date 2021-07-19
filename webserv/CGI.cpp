#include "CGI.hpp"
#include <sys/wait.h>

CGI::~CGI() {}

void CGI::executeCGI(Request & request, Response & response) {
  char **envp = setEnv(request, response);
  pid_t pid;
  int status;
  std::string cgi_fin_path = "/Users/cbach/CLionProjects/webserv/webserv/site/cgi/cgi_fin";
  std::string cgi_fout_path = "/Users/cbach/CLionProjects/webserv/webserv/site/cgi/cgi_fout";

  int fin = open(cgi_fin_path.c_str(), O_RDWR | O_TRUNC | O_CREAT, 0777);
  int fout = open(cgi_fout_path.c_str(), O_RDWR | O_TRUNC | O_CREAT, 0777);
  write(fout, request.body.c_str(), request.body.length());
  lseek(fout, 0, SEEK_SET);
  std::cout << "Forking CGI execution with target = " << request.request_line["target"] << std::endl;
  pid = fork();
  if (pid == -1) {
    throw std::runtime_error("error");
  } else if (pid == 0) {
    chdir(request.server_config.root.c_str());
    dup2(fout, STDIN_FILENO);
    dup2(fin, STDOUT_FILENO);
    char **kek = new char*[3];
    std::string path = request.server_config.root + "/cgi/cgi_tester";
    kek[0] = strdup(path.c_str());
    kek[1] = strdup(response.fullPath_.c_str());
    kek[2] = NULL;
    std::cerr << "execve path = " << kek[0] << std::endl;
    std::cerr << "execve 1st argument path = " << kek[1] << std::endl;
    if (execve(kek[0], kek, envp) == -1) {
//      throw std::runtime_error("Error: execve");
        std::cerr << "CGI FUCKED UP" << std::endl;
        std::cerr << errno << strerror(errno) << std::endl;
    }
    std::cerr << "END OF EXECVE CALL" << std::endl;
    exit(0);
  }
  std::cout << "WAITING FOR EXECVE PROCESS" << std::endl;
  waitpid(pid, &status, 0);
  struct stat file;
  fstat(fin, &file);
  lseek(fin, 0, SEEK_SET);
  response.body.resize(file.st_size);
  char *pizda = reinterpret_cast<char *>(malloc(100000));
  read(fin, const_cast<char *>(response.body.c_str()), response.body.capacity());
  close(fin);
  close(fout);
  std::cout << "END OF GETTING CGI RESPONSE" << std::endl;
  response.body.replace(0, 7, request.request_line["version"]);
  response.body += "\r\n\r\n";
//  response.body.replace(response.body.size() - 1, 1, 1, EOF);
  std::cout << "CGI RETURNED: " << response.body.substr(0, 100) << "... (" << response.body.size() << " bytes)" << std::endl;
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
  if (req.body.size()) {
    tmpEnv["CONTENT_LENGTH="] = req.headers["Content-Length"];
    it = req.headers.find("Content-Type");
    if (it != end)
      tmpEnv["CONTENT_TYPE="] = it->second;
  }
  tmpEnv["GATEWAY_INTERFACE="] = "CGI/1.1";
  it = req.request_line.find("target");
  pos = it->second.find('?');
  std::string path_info =   (pos == it->second.size() ? it->second : it->second.substr(0, pos));
  tmpEnv["PATH_INFO="] = path_info;
  std::cerr << "PATH_INFO = " << path_info << std::endl;
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
	tmpEnv["HTTP_X_SECRET_HEADER_FOR_TEST="] = "1";
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
