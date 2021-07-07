#include "Response.hpp"

Response::Response(){}

const std::string &Response::SetResponseLine(const std::map<std::string, std::string> &request_line, conf &con) {
	
	char webpage[] =
			"HTTP/1.1 200 OK\r\n"
//			"Content-Length 154\r\n"
			"Content-Type text/html; charset=UTF-8\r\n\r\n"
			"<!DOCTYPE html>\r\n"
			"<html><head><title>webserv</title>\r\n"
			"<style>body {background-color: #FFFF00}</style></head>\r\n"
			"<body><center><h1>Bla bla!</h1><br>\r\n";
	
	if (request_line.find("version")->second == "HTTP/1.1")
		this->response_line["version"] = request_line.find("version")->second;
	else{
		std::cout << "HTTP version error!\n";
		exit(1);
	}
	if(request_line.find("method")->second == "GET"){
//		responseLine
		if (SearchForDir(con.root + request_line.find("target")->second)){
			this->response_line["status_code"] = "200";
			this->response_line["status"] = GetStatusText(this->response_line.find("status_code")->second);
//			bodyLine
			std::ifstream fin;
			std::string path;
			std::string line;
			path = con.root + request_line.find("target")->second;
			if (path[path.size() - 1] == '/')
				path += "index.html";
			else
				path += "/index.html";
			fin.open(path);
			while (fin){
				getline(fin, line);
				if (fin)
					this->body += line + "\r\n";
				
			}
			//		headerLine
			this->headers["Content-Type"] = "text/html; charset=UTF-8";
			this->headers["Content-Length"] = std::to_string(this->body.size() + 4);
		}
//		else if (SearchForFile(con.root + request_line.find("target")->second)){
//			this->response_line["status_code"] = "200";
//			this->response_line["status"] = GetStatusText(this->response_line.find("status_code")->second);
//
//			std::ifstream fin;
//			std::string path;
//			std::string line;
//			path = con.root + request_line.find("target")->second;
//			fin.open(path);
//			while (fin){
//				getline(fin, line);
//				if (fin)
//					this->body += line +  "\r\n";
//			}
//
//			this->headers["Content-Type"] = "text/html; charset=UTF-8";
//			this->headers["Content-Length"] = std::to_string(this->body.size());
//		}
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
//	if (SearchForDir(con.root + request_line.find("target")->second)) {
//		if (request_line.find("method")->second == "GET") {
//			std::string path = "myFile.txt";
//			std::ofstream fout;
//			fout.open(path.c_str());
//			this->response_line["status_code"] = "200";
//			this->response_line["status"] = GetStatusText(this->response_line.find("status_code")->second);
//			if (fout.is_open()){
//				fout << this->response_line.find("version")->second << " "
//					 << this->response_line.find("status_code")->second << " "
//					 << this->response_line.find("status")->second << "\r\n"
//					 << "Content-Type text/html; charset=UTF-8\r\n\r\n";
//				std::string line;
//				std::ifstream fin;
//				fin.open(con.root + request_line.find("target")->second + "/index.html");
//				while(fin) {
//					std::cout << fin << std::endl;
//					getline(fin, line);
//					if (fin)
//						fout << line + "\n";
//				}
//				std::cout << fin << std::endl;
//				fin.close();
////		Response StatusLine viewer
//				std::cout << this->response_line.find("version")->second << " "
//						  << this->response_line.find("status_code")->second << " "
//						  << this->response_line.find("status")->second << "\r\n";
////			}
//			std::ifstream fin;
//			fin.open("/Users/ljerrica/Desktop/webserv/cmake-build-debug/myFile.txt");
//			std::string str;
//			std::string line;
//			while(fin){
//				getline(fin, line);
//				if (fin)
//					 str += line +  "\n";
//			}
//			return str.c_str();
//			close(fout);
//		}
//		else if (request_line.find("method")->second == "POST" || request_line.find("method")->second == "DELETE"){
//			this->response_line["status_code"] = "201";
//			this->response_line["status"] = GetStatusText((this->response_line.find("status_code")->second));
//		}
//	}
//	else if (SearchForFile(con.root + request_line.find("target")->second)){
//		if (request_line.find("method")->second == "GET") {
//			std::string line;
//			std::ifstream fin;
//			fin.open(con.root + request_line.find("target")->second);
//			while(fin){
//				getline(fin, line);
//			}
//		}f (fout.is_open()){
//			fout << this->response_line.find("version")->second << " "
//				 << this->response_line.find("status_code")->second << " "
//				 << this->response_line.find("status")->second << "\r\n";
//
//			i
////		Response StatusLine viewer
//			std::cout << this->response_line.find("version")->second << " "
//					  << this->response_line.find("status_code")->second << " "
//					  << this->response_line.find("status")->second << "\r\n";
//		}
//	}
//	else{
//		this->response_line["status_code"] = "404";
//		this->response_line["status"] = GetStatusText(this->response_line.find("status_code")->second);
//	}
	return SendResponse();
	return webpage;
}

void Response::freeResponse() {
	this->response_line.clear();
	this->headers.clear();
	this->body.clear();
	this->status_codes.clear();
}

const std::string &Response::SendResponse() {
	std::string response;
	std::map<std::string, std::string>::iterator begin;
	
	response = this->response_line.find("version")->second + " ";
	response += this->response_line.find("status_code")->second + " ";
	response += this->response_line.find("status")->second + "\r\n";
	for (begin = this->headers.begin(); begin != this->headers.end(); begin++) {
//		if (begin == this->headers.begin())
//			response += "\r\n";
		response += begin->first + " ";
		response += begin->second + "\r\n";
	}
	response += "\r\n";
	response += this->body;
	
	std::cout << response;
	return response;
}

const std::string Response::GetStatusText(std::string code) {
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
	if (statusText.find(code) != statusText.end())
		return statusText.find(code)->second;
	std::cout << "status code error!" << std::endl;
	exit(1);
}

