#include "allowed_library_includes.hpp"
#include "Server.hpp"
#include "parser.hpp"
#include <sys/wait.h>

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


char** mapToCString() {
  chdir("/Users/cbach/CLionProjects/webserv/webserv/site/");
  std::map<std::string, std::string> tmpEnv;
  tmpEnv["CONTENT_LENGTH="] = "3";
  tmpEnv["CONTENT_TYPE="] = "test/file";
  tmpEnv["GATEWAY_INTERFACE="] = "CGI/1.1";
//  tmpEnv["PATH_INFO="] = "directory/youpi.bla";
  tmpEnv["PATH_INFO="] = "YoupiBanane/directory/youpi.bla";
  tmpEnv["PATH_TRANSLATED="] = "/Users/cbach/CLionProjects/webserv/webserv/site/YoupiBanane/directory/youpi.bla";
  tmpEnv["QUERY_STRING="] = "";
//  tmpEnv["REDIRECT_STATUS"] = "200";
  tmpEnv["REMOTE_ADDR="] = "174.127.0.0" ;
//  tmpEnv["REMOTE_IDENT="] = "basic";
//  tmpEnv["REMOTE_USER="] = "?????";
  tmpEnv["REQUEST_METHOD="] = "POST";
  tmpEnv["REQUEST_URI="] = "YoupiBanane/directory/youpi.bla";
  tmpEnv["SCRIPT_NAME="] = "cgi/cgi_tester";
  tmpEnv["SCRIPT_FILENAME="] = "/Users/cbach/CLionProjects/webserv/webserv/site/cgi/cgi_tester";

//todo rework with inet_ntoa
//  tmpEnv["SERVER_NAME="] = std::to_string(_con.host);
  tmpEnv["SERVER_NAME="] = "127.0.0.1";
  tmpEnv["SERVER_PORT="] = "8080";
  tmpEnv["SERVER_PROTOCOL="] = "HTTP/1.1";
  tmpEnv["SERVER_SOFTWARE="] = "kekers228/v4.20";
//	tmpEnv["HTTP_X_SECRET_HEADER_FOR_TEST="] = "1";
  tmpEnv["HTTP_X_SECRET_HEADER_FOR_TEST="] = "1";
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
  char **envp = mapToCString();
  pid_t pid;
  int status;
  int fd[2];
  int stdoutt = dup(STDOUT_FILENO);
  int stdinn = dup(STDIN_FILENO);
  pipe(fd);
  pid = fork();

  if (pid == -1) {
    throw std::runtime_error("error");
  } else if (pid == 0) {
//    dup2(fd[1], STDOUT_FILENO);
    dup2(fd[0], STDIN_FILENO);
//    close(fd[1]);
    dup2(fd[1], STDOUT_FILENO);
    std::string hui = "/Users/cbach/CLionProjects/webserv/webserv/site/cgi/cgi_tester";
    char **kek = new char*[2];
    kek[0] = strdup("/Users/cbach/CLionProjects/webserv/webserv/site/cgi/cgi_tester");
    kek[1] = NULL;
    if (execve(kek[0], kek, envp) == -1) {
      write(2, strerror(errno), strlen(strerror(errno)));
      write(2, "hui\n", 4);

    }

    exit(0);
  }
  else {
    std::string pizda;
//    dup2(fd[1], STDOUT_FILENO);
//    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    write(fd[1], "123", strlen("123"));
    close(fd[1]);
//    dup2(stdoutt, STDOUT_FILENO);
//    std::cout << "end" << std::endl;
    wait(NULL);
//    std::string tmp;
//    while(std::cin >> tmp)
//      pizda += tmp;
std::getline(std::cin, pizda, '\0');
        dup2(stdinn, STDIN_FILENO);
    return pizda;
  }
}



int main(int argc, char **argv) {
  if (argc > 1) // for tests
    exit(0);
//  std::string res = testCGI();
//  std::cout << res;
  ServerConfig config = parsConf();
  const ServerConfig DEFAULT_CONFIG =
      {0, 8080, std::string(),
       std::vector<std::string>(),
       std::vector<error>(),
       8096,
       std::vector<std::string>(),
       std::vector<location>(),};
  std::vector<ServerConfig> serverConfigs;
//  serverConfigs.push_back(DEFAULT_CONFIG);
serverConfigs.push_back(config);
  Server server(serverConfigs, 65536);
  server.Run();

  return 0;
}