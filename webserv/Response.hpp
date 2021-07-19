#ifndef WEBSERV_RESPONSE_HPP_
#define WEBSERV_RESPONSE_HPP_

#include "allowed_library_includes.hpp"
#include "Request.hpp"
#include "parser.hpp"
class CGI;
#include "CGI.hpp"

class Response {
public:
	std::string							SetResponseLine();
	void								SetHeader(const std::string &key, const std::string &value);
	explicit							Response(Request & request);
	std::string							GetStatusText(std::string code);
	void								GetContentType();
	bool								MakeDirectory();
	bool 								OpenOrCreateFile();
	std::string							SendResponse();
	bool								ResponseBuilder(const std::string &path, const std::string &status_code);
	bool								HTTPVersionControl();
	void								SetErrorResponse(const std::string &status_code);
	void								GetRequest();
	void								PostRequest();
	void								HeadRequest();
	void								PutRequest();
	void								DeleteRequest();
	bool								CheckMethodCorrectness();
	bool								CheckLocationCorrectness();
	bool								CheckLocationMethods();
	void								freeResponse();
	void								SetStatus(const std::string &code);
	void								SetHeaders();
	void								ErrorHandler(const std::string &status_code);
	std::string							GetTimeGMT();
	bool								SetBody(const std::string &path);
	bool								GetBody(const std::string &path);
//	void	createCGI(const std::map<std::string, std::string> &request_line, const ServerConfig &con,
//				 const std::map<std::string, std::string> &headers);
// private:
	Request								request_;
	std::map<std::string, std::string>	response_line;
	std::map<std::string,std::string>	headers;
	std::map<std::string, std::string>	status_codes;
	std::string							body; //????????
	size_t								status_code;
	location							location_;
	std::map<std::string, std::string>	content_type_;
	std::map<std::string, std::string>	status_text_;
	std::string							cleanTarget_;
	std::string							fullPath_;
	std::string                         fullFullPath_;
	const ServerConfig					&ServerConf_;
	bool								failed;

//private:
	bool								_SearchForFile(const std::string &path);
	bool								_SearchForDir();
	void								_createHTMLAutoIndex(DIR *dir);
	const std::string _getTimeModify(const std::string &path);
	void								CorrectPath();

};


#endif //WEBSERV_RESPONSE_HPP_
