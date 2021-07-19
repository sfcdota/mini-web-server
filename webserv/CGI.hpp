//
// Created by Knot Fonda on 7/10/21.
//

#ifndef WEBSERV_CGI_HPP_
#define WEBSERV_CGI_HPP_

//#include "allowed_library_includes.hpp"
////#include "Server.hpp"
class Response;
#include "Response.hpp"
// Сервер отправляет данные программе CGI с помощью переменных среды

// Задача:
// Сформировать execve->env
// При использовании метода GET данные отправляются в переменную QUERY_STRING
// При использовании метода GET данные отправляются в body (CONTENT_LENGTH)
//
//канонические переменные:
//AUTH_TYPE
//CONTENT_LENGTH
//CONTENT_TYPE
//GATEWAY_INTERFACE
//PATH_INFO
//PATH_TRANSLATED
//QUERY_STRING
//REMOTE_ADDR
//REMOTE_IDENT
//REMOTE_USER
//REQUEST_METHOD
//REQUEST_URI
//SCRIPT_NAME
//SERVER_NAME
//SERVER_PORT
//SERVER_PROTOCOL
//SERVER_SOFTWARE
//HTTP_X_SECRET_HEADER_FOR_TEST = 1

class CGI {
public:
  CGI() {};
  ~CGI();
  static void executeCGI(Request & request, Response & response);
  static char ** mapToCString(std::map<std::string, std::string> &tmpEnv);
  static char** setEnv(Request &req, Response & response);
  static std::string & translate_path(std::string & path);
  static void deleteENVP(char **envp);

};

#endif //WEBSERV_CGI_HPP_
