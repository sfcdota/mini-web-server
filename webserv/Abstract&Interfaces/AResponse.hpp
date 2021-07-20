#ifndef WEBSERV_ARESPONSE_HPP_
#define WEBSERV_ARESPONSE_HPP_
#include "../Request.hpp"
class AResponse {
public:
//	std::string			GetResponse();
 protected:
	explicit AResponse(const Request & request);
	virtual ~AResponse();
	virtual const std::string GetResponse() const = 0;
	virtual void								GetRequest() const = 0;
	virtual void								PostRequest() const = 0;
	virtual void								HeadRequest() const = 0;
	virtual void								PutRequest() const = 0;
	virtual void								DeleteRequest() const = 0;
	
	std::string _response;
	const Request & _request;
	const ServerConfig & _serverConf;
	std::map<std::string, std::string>	response_line;
	std::map<std::string,std::string>	headers;
	std::string							body;
  
};
#endif //WEBSERV_ARESPONSE_HPP_
