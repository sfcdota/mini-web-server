#include "Response.hpp"

Response::Response(){}

void Response::SetResponseLine(const std::map<std::string, std::string> &request_line, conf &con) {
	

	this->response_line["version"] = request_line.find("version")->second;
	if (SearchForDir(con.root + request_line.find("target")->second)) {
		if (request_line.find("method")->second == "GET") {
			std::string path = "myFile.txt";
			std::ofstream fout;
			fout.open(path.c_str());
			this->response_line["status_code"] = "200";
			this->response_line["status"] = GetStatusText(this->response_line.find("status_code")->second);
			if (fout.is_open()){
				fout << this->response_line.find("version")->second << " "
					 << this->response_line.find("status_code")->second << " "
					 << this->response_line.find("status")->second << "\r\n"
					 << "Content-Type text/html; charset=UTF-8\r\n\r\n";
				std::string line;
				std::ifstream fin;
				fin.open(con.root + request_line.find("target")->second + "/index.html");
				while(fin) {
					getline(fin, line);
					fout << line;
				}
//		Response StatusLine viewer
				std::cout << this->response_line.find("version")->second << " "
						  << this->response_line.find("status_code")->second << " "
						  << this->response_line.find("status")->second << "\r\n";
			}
//			close(fout);
		}
		else if (request_line.find("method")->second == "POST" || request_line.find("method")->second == "DELETE"){
			this->response_line["status_code"] = "201";
			this->response_line["status"] = GetStatusText((this->response_line.find("status_code")->second));
		}
	}
	else if (SearchForFile(con.root + request_line.find("target")->second)){
		if (request_line.find("method")->second == "GET") {
			std::string line;
			std::ifstream fin;
			fin.open(con.root + request_line.find("target")->second);
			while(fin){
				getline(fin, line);
			}
		}
//		if (fout.is_open()){
//			fout << this->response_line.find("version")->second << " "
//				 << this->response_line.find("status_code")->second << " "
//				 << this->response_line.find("status")->second << "\r\n";
//
////		Response StatusLine viewer
//			std::cout << this->response_line.find("version")->second << " "
//					  << this->response_line.find("status_code")->second << " "
//					  << this->response_line.find("status")->second << "\r\n";
//		}
	}
	else{
		this->response_line["status_code"] = "404";
		this->response_line["status"] = GetStatusText(this->response_line.find("status_code")->second);
	}
	
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

