#ifndef WEBSERV_RESPONSE_HPP_
#define WEBSERV_RESPONSE_HPP_

#include "allowed_library_includes.hpp"
#include "Request.hpp"
#include "parser.hpp"
#include "CGI.hpp"

class Response {
public:
	explicit Response(Request & request_);
	std::string SetResponseLine(const ServerConfig &con);
	std::string GetStatusText(std::string code);
	std::string SendResponse();
	void	ResponseBuilder(const std::string &path, const std::string &status_code);
	void HTTPVersionControl();
	void	GetRequest(const ServerConfig &con);
	void	PostRequest(const ServerConfig &con);
	void	HeadRequest(const ServerConfig &con);
	void	freeResponse();
//	void	createCGI(const std::map<std::string, std::string> &request_line, const ServerConfig &con,
//				 const std::map<std::string, std::string> &headers);
// private:
	Request request_;
	std::map<std::string, std::string> response_line;
	std::map<std::string,std::string> headers;
	std::map<std::string, std::string> status_codes;
	std::string body; //????????
	size_t status_code;
	ServerConfig s;
	bool failed;
};


#endif //WEBSERV_RESPONSE_HPP_
