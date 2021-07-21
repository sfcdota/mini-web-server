#ifndef WEBSERV_ARESPONSE_HPP_
#define WEBSERV_ARESPONSE_HPP_
#include "../Request.hpp"
class AResponse {
public:
//	std::string			GetResponse();
 protected:
	explicit AResponse(const Request & request);
	virtual ~AResponse();
	virtual const std::string GetResponse() = 0;
	virtual void								GetRequest() = 0;
	virtual void								PostRequest() = 0;
	virtual void								HeadRequest() = 0;
	virtual void								PutRequest() = 0;
	virtual void								DeleteRequest() = 0;
	
	const Request & _request;
	const ServerConfig & _serverConf;
	std::map<std::string, std::string>	response_line;
	std::map<std::string,std::string>	headers;
	std::string							body_;
  
};
#endif //WEBSERV_ARESPONSE_HPP_
