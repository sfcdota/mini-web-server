#ifndef WEBSERV_RESPONSE_HPP_
#define WEBSERV_RESPONSE_HPP_

#include "allowed_library_includes.hpp"
#include "Request.hpp"
#include "parser.hpp"
#include "CGI.hpp"

class Response {
public:
	std::string SetResponseLine();
	explicit Response(Request & request);
	std::string GetStatusText(std::string code);
	std::string SendResponse();
	void	ResponseBuilder(const std::string &path, const std::string &status_code);
	void HTTPVersionControl();
	void GetRequest();
	void PostRequest();
	void HeadRequest();
	bool 	CheckMethodCorrectness();
	bool 	CheckLocationCorrectness();
	bool	CheckLocationMethods();
	void	freeResponse();
	void SetStatus(std::string code);
//	void	createCGI(const std::map<std::string, std::string> &request_line, const ServerConfig &con,
//				 const std::map<std::string, std::string> &headers);
// private:
	Request request_;
	std::map<std::string, std::string> response_line;
	std::map<std::string,std::string> headers;
	std::map<std::string, std::string> status_codes;
	std::string body; //????????
	size_t status_code;
	location location_;
	std::string cleanTarget_;
	std::string fullPath_;
	const ServerConfig &ServerConf_;
	
	bool failed;
private:
	bool _SearchForFile(const std::string &path);
	bool _SearchForDir();
	void _createHTMLAutoIndex(DIR *dir);
	std::string _getTimeModify(std::string path);
	
};


#endif //WEBSERV_RESPONSE_HPP_
