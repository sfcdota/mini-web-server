#include "Response.hpp"

Response::Response(){}

void Response::ResponseBuilder(const std::string &path, const std::string &status_code) {
//	responseLine
	this->response_line["status_code"] = status_code;
	this->response_line["status"] = GetStatusText(this->response_line.find("status_code")->second);
//	bodyLine
	std::ifstream fin(path, std::ios::in|std::ios::binary|std::ios::ate);
	int size;
	if (fin.is_open())
	{
		fin.seekg(0, std::ios::end);
		size = fin.tellg();
		char *contents = new char [size];
		fin.seekg (0, std::ios::beg);
		fin.read (contents, size);
		fin.close();
		std::string str(contents, size);
		delete [] contents;
		this->body = str;
	}
//	headerLine
	this->headers["Content-Type"] = "text/html; charset=utf-8";
	this->headers["Content-Length"] = std::to_string(this->body.size());
}

void Response::HTTPVersionControl(const std::string &httpVersion) {
	if (httpVersion == "HTTP/1.1")
		this->response_line["version"] = httpVersion;
	else{
		std::cout << "HTTP version error!\n";
		exit(1);
	}
}

void Response::GetRequest(const std::map<std::string, std::string> &request_line, const ServerConfig &con) {
	if (SearchForDir(con.root + request_line.find("target")->second)){
		std::string path = con.root + request_line.find("target")->second;
		if (path[path.size() - 1] == '/')
			ResponseBuilder(path + "index.html", "200");
		else
			ResponseBuilder(path + "/index.html", "200");
	}
	else if (SearchForFile(con.root + request_line.find("target")->second))
		ResponseBuilder(con.root + request_line.find("target")->second, "200");
	else{
		std::string path = con.root;
		if (path[path.size() - 1] == '/')
			ResponseBuilder(path + "errors/404.html", "404");
		else
			ResponseBuilder(path + "/errors/404.html", "404");
	}
}

std::string Response::SetResponseLine(const std::map<std::string, std::string> &request_line, const ServerConfig &con) {
	HTTPVersionControl(request_line.find("version")->second);
	if(request_line.find("method")->second == "GET")
		GetRequest(request_line,  con);

	return SendResponse();
}

void Response::freeResponse() {
	this->response_line.clear();
	this->headers.clear();
	this->body.clear();
	this->status_codes.clear();
}

std::string Response::SendResponse() {
	std::string response;
	std::map<std::string, std::string>::iterator begin;
	
	response = this->response_line.find("version")->second + " ";
	response += this->response_line.find("status_code")->second + " ";
	response += this->response_line.find("status")->second /*+ "\r\n"*/;
	for (begin = this->headers.begin(); begin != this->headers.end(); begin++) {
		if (begin == this->headers.begin())
			response += "\r\n";
		response += begin->first + ": ";
		response += begin->second + "\r\n";
	}
	response += "\r\n";
	response += this->body;
	
	return response;
}

std::string Response::GetStatusText(std::string code) {
	std::map<std::string, std::string> statusText;
	statusText["100"] = "Continue";
	statusText["101"] = "Switching Protocols";
	statusText["102"] = "Processing";
	statusText["103"] = "Early Hints";
	statusText["200"] = "OK";
	statusText["201"] = "Created";
	statusText["202"] = "Accepted";
	statusText["203"] = "Non_Authoritative Information";
	statusText["204"] = "No Content";
	statusText["205"] = "Reset Content";
	statusText["300"] = "Multiple Choices";
	statusText["301"] = "Moved Permanently";
	statusText["302"] = "Found";
	statusText["303"] = "See Other";
	statusText["305"] = "Use Proxy";
	statusText["306"] = "(Unused)";
	statusText["307"] = "Temporary Redirect";
	statusText["400"] = "Bad Request";
	statusText["402"] = "Payment Required";
	statusText["403"] = "Forbidden";
	statusText["404"] = "Not Found";
	statusText["405"] = "Method Not Allowed";
	statusText["406"] = "Not Acceptable";
	statusText["408"] = "Request Timeout";
	statusText["409"] = "Conflict";
	statusText["410"] = "Gone";
	statusText["411"] = "Length Required";
	statusText["413"] = "Payload Too Large";
	statusText["414"] = "URI Too Long";
	statusText["415"] = "Unsupported Media Type";
	statusText["417"] = "Expectation Failed";
	statusText["426"] = "Upgrade Required";
	statusText["500"] = "Internal Server Error";
	statusText["501"] = "Not Implemented";
	statusText["502"] = "Bad Gateway";
	statusText["503"] = "Service Unavailable";
	statusText["504"] = "Gateway Timeout";
	statusText["505"] = "HTTP Version Not Supported";
	if (statusText.find(code) != statusText.end())
		return statusText.find(code)->second;
	std::cout << "status code error!" << std::endl;
	exit(1);
}

