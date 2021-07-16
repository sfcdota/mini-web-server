#include "allowed_library_includes.hpp"
#include "Server.hpp"
#include "parser.hpp"
/*
 * /directory/youpi.bla
CONTENT_LENGTH=100000000
CONTENT_TYPE=test/file
GATEWAY_INTERFACE=CGI/1.1
PATH_INFO=/directory/youpi.bla
PATH_TRANSLATED=/Users/cbach/CLionProjects/webserv/webserv/site/directory/youpi.bla
QUERY_STRING=
REMOTE_ADDR=174.127.0.0
REQUEST_METHOD=POST
REQUEST_URI=/directory/youpi.bla
SCRIPT_FILENAME=/Users/cbach/CLionProjects/webserv/webserv/site/cgi/cgi_tester
SCRIPT_NAMEcgi/cgi_tester
SERVER_NAME=127.0.0.1
SERVER_PORT=8080
SERVER_PROTOCOL=HTTP/1.1
SERVER_SOFTWARE=kekers228/v4.20
 */


char** mapToCString(std::map<std::string, std::string> &tmpEnv) {
  char **_env;
  _env = reinterpret_cast<char **>(calloc(tmpEnv.size() + 1, sizeof(char *)));
  std::map<std::string, std::string>::iterator it = tmpEnv.begin();
  for (int index = 0; it != tmpEnv.end(); ++it, ++index) {
    _env[index] = strdup((it->first + it->second).c_str());
    std::cout << _env[index] << std::endl;
  }
  return _env;
}



std::string testCGI() {
  chdir("/Users/cbach/CLionProjects/webserv/webserv/site/");
  std::map<std::string, std::string> tmpEnv;
  tmpEnv["CONTENT_LENGTH="] = "5";
  tmpEnv["CONTENT_TYPE="] = "test/file";
  tmpEnv["GATEWAY_INTERFACE="] = "CGI/1.1";
  tmpEnv["PATH_INFO="] = "/directory/youpi.bla";
  tmpEnv["PATH_TRANSLATED="] = "/Users/cbach/CLionProjects/webserv/webserv/site/directory/youpi.bla";
  tmpEnv["QUERY_STRING="] = "";
//  tmpEnv["REDIRECT_STATUS"] = "200";
  tmpEnv["REMOTE_ADDR="] = "174.127.0.0" ;
//  tmpEnv["REMOTE_IDENT="] = "basic";
//  tmpEnv["REMOTE_USER="] = "?????";
  tmpEnv["REQUEST_METHOD="] = "POST";
  tmpEnv["REQUEST_URI="] = "/YoupiBanane/directory/youpi.bla";
  tmpEnv["SCRIPT_NAME="] = "cgi/cgi_tester";
  tmpEnv["SCRIPT_FILENAME="] = "/Users/cbach/CLionProjects/webserv/webserv/site/cgi/cgi_tester";

//todo rework with inet_ntoa
//  tmpEnv["SERVER_NAME="] = std::to_string(_con.host);
  tmpEnv["SERVER_NAME="] = "127.0.0.1";
  tmpEnv["SERVER_PORT="] = "8080";
  tmpEnv["SERVER_PROTOCOL="] = "HTTP/1.1";
  tmpEnv["SERVER_SOFTWARE="] = "kekers228/v4.20";
//	tmpEnv["HTTP_X_SECRET_HEADER_FOR_TEST="] = "1";
  char **envp = mapToCString(tmpEnv);
  pid_t pid;
  int status;
  int fd[2];
  pipe(fd);
  pid = fork();
//  dup2(fd[1], STDOUT_FILENO);

dup2(STDIN_FILENO, fd[0]);
  if (pid == -1) {
    throw std::runtime_error("error");
  } else if (pid == 0) {
    dup2(fd[0], STDIN_FILENO);
    dup2(fd[1], STDOUT_FILENO);
//    close(fd[1]);
    std::string hui = "/Users/cbach/CLionProjects/webserv/webserv/site/cgi/cgi_tester";
    char **kek = new char*[1];
    kek[0] = "/Users/cbach/CLionProjects/webserv/webserv/site/cgi/cgi_tester";
//    char* kek = const_cast<char *>(hui.c_str());
//    dup2(STDIN_FILENO, STDOUT_FILENO);
    if (execve(kek[0], kek, envp) == -1) {
      std::cout << errno << " " << strerror(errno) << std::endl;
      throw std::runtime_error("Error: execve");
    }
    std::cout << "PRIVETIKI" << std::endl;
    exit(0);
  }
//  close(fd[0]);
//  dup2(fd[1], STDOUT_FILENO);
//  std::cout << "POST /directory/youpi.bla HTTP/1.1\r\nContent-Length: 5\r\nContent-Type: test/file\r\n\r\neeeee\r\n\r\n";
  write(fd[0], "eeeee", 5);
  waitpid(pid, &status, 0);
//  dup2(fd[1], STDIN_FILENO);
  std::string result_dst;
  char *hui = (char*)malloc(10000);
  size_t statuss = read(fd[1], hui, 100);
  std::cout << hui;
  close(fd[1]);
  return "ha";
}



int main(int argc, char **argv) {
  if (argc > 1) // for tests
    exit(0);

  std::string output = testCGI();
  std::cout << output << std::endl;
//  ServerConfig config = parsConf();
//  const ServerConfig DEFAULT_CONFIG =
//      {0, 8080, std::string(),
//       std::vector<std::string>(),
//       std::vector<error>(),
//       8096,
//       std::vector<std::string>(),
//       std::vector<location>(),};
//  std::vector<ServerConfig> serverConfigs;
////  serverConfigs.push_back(DEFAULT_CONFIG);
//serverConfigs.push_back(config);
//  Server server(serverConfigs, 2048);
//  server.Run();
//  std::string kek = "sec-ch-ua: \" Not;A Brand\";v=\"99\", \"Google Chrome\";v=\"91\", \"Chromium\";v=\"91\"";
//
//  std::string kek2 = "sec-ch-ua-mobile: ?0";
//  size_t pos = 0;
//  std::cout << MessageValidator::ValidHeader(kek2, pos) << std::endl;

  return 0;
}