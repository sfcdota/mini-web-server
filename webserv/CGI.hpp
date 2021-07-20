//
// Created by Knot Fonda on 7/10/21.
//

#ifndef WEBSERV_CGI_HPP_
#define WEBSERV_CGI_HPP_

class Response;
#include "Response.hpp"
#include "MessageValidator.hpp"
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>

enum class CGILoggingOptions {
  ZERO,
  ENVS,
  EXECVE_PATH,
  EXECVE_ARGUMENT,
  RESPONSE,
  FULL_RESPONSE,
};

class CGI: ILogger<CGILoggingOptions> {
public:
  CGI() {};
  ~CGI();
  const std::string PrintLog(const int & logginglevel, const CGILoggingOptions & option) const;
  static void executeCGI(const Request & request, const Response & response);
  static char * const * mapToCString(const std::map<std::string, std::string> &tmpEnv);
  static char * const * setEnv(const Request &req, const Response & response);
  static std::string & translate_path(std::string & path);
  static void deleteENVP(const char * const *envp);
  static std::string cgi_response;
};

#endif //WEBSERV_CGI_HPP_
