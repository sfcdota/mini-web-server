#include "Response.hpp"

Response::Response(){ }

void Response::SetResponseLine(const std::map<std::string, std::string> &request_line) {
	std::cout << "it work!" << std::endl;

	this->response_line["version"] = request_line.find("version")->second;
	if (request_line.find("method")->second == "GET" && request_line.find("target")->second == "/") {
		this->response_line["status_code"] = "200";
		this->response_line["status"] = "OK";
	}

}

const std::string Response::GetStatusText(std::string code) {
	std::map<std::string, std::string> statusText;
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
	statusText["306"] = "(Unused";
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
	statusText["200"] = "OK";
	statusText["200"] = "OK";
	statusText["200"] = "OK";
	statusText["200"] = "OK";
	statusText["200"] = "OK";


}

