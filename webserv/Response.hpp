#ifndef WEBSERV_RESPONSE_HPP_
#define WEBSERV_RESPONSE_HPP_

#include "Request.hpp"
#include "parser.hpp"
class CGI;
#include "CGI.hpp"
#include "Abstract&Interfaces/AResponse.hpp"
#include <dirent.h>
#include <sstream>
#include <fstream>

enum class ResponseLoggingOptions {
  ZERO,
};

class Response: public AResponse, ILogger<ResponseLoggingOptions> {
public:
	Response(const Request &request);
	virtual const std::string GetResponse();
    const std::string PrintLog(const ResponseLoggingOptions &option) const;
    ~Response();
private:
	void								SetHeader(const std::string &key, const std::string &value);
	std::string							GetStatusText(std::string code);
	bool								MakeDirectory();
	bool 								OpenOrCreateFile();
	const std::string					SendResponse();
	bool								SetResponse(const std::string &path, const std::string &status_code);
	bool								HTTPVersionControl();
	virtual void						GetRequest();
	virtual void						PostRequest();
	virtual void						HeadRequest();
	virtual void						PutRequest();
	virtual void						DeleteRequest();
	bool								CheckMethodCorrectness();
	bool								CheckLocationCorrectness();
	bool								CheckLocationMethods();
	bool								IsRequestCorrect();
	void								SetStatus(const std::string &code);
	void								ErrorHandler(const std::string &status_code);
	std::string							GetTimeGMT();
	bool								SetBody(const std::string &path);
	bool								FillBody(const std::string &path);
	std::string							PathBuilder(const std::string & path);


	location							location_;
	std::map<std::string, std::string>	content_type_;
	std::string							cleanTarget_;
	std::string							fullPath_;
	const ServerConfig&					ServerConf_;


	bool								_SearchForFile(const std::string &path);
	bool								_SearchForDir();
	void								_createHTMLAutoIndex(DIR *dir);
	const std::string					_getTimeModify(const std::string &path);
public:
	const Request &						GetRequestClass() const ;
	const std::string &					GetBody() const ;
	const std::string &					GetFullPath() const ;
	const location &					GetLocation() const ;
	const std::string &					GetCleanTarget() const ;
	const std::map<std::string, std::string> &	GetHeaders() const ;
};


#endif //WEBSERV_RESPONSE_HPP_
