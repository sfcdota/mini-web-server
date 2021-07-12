#include "Response.hpp"

Response::Response(Request & request): request_(request), ServerConf_(request_.server_config) {}


void Response::ResponseBuilder(const std::string &path, const std::string &status_code) {
//	responseLine
	this->response_line["status_code"] = status_code;
	this->response_line["status"] = GetStatusText(this->response_line.find("status_code")->second);
//	bodyLine
	std::ifstream fin(path, std::ios::in|std::ios::binary|std::ios::ate);
	int size;
	if (fin.is_open()) {
		fin.seekg(0, std::ios::end);
		size = fin.tellg();
		char *contents = new char[size];
		fin.seekg (0, std::ios::beg);
		fin.read (contents, size);
		fin.close();
		std::string str(contents, size);
		delete [] contents;
		this->body = str;
	}
//	headerLine
	if (path.substr(path.find('.') + 1) == "html")
		this->headers["Content-Type"] = "text/html; charset=utf-8";
	else
		this->headers["Content-Type"] = "image/*";
	this->headers["Content-Length"] = std::to_string(this->body.size());
}

void Response::HTTPVersionControl() {
	std::cout << request_.request_line.find("version")->second << std::endl;
	if (request_.request_line.find("version")->second == "HTTP/1.1") {
		this->response_line["version"] = "HTTP/1.1";
	} else {
		std::cout << "HTTP version error!\n";
		exit(1);
	}
}

void Response::GetRequest() {
	if (_SearchForDir()){
		if (this->fullPath_[this->fullPath_.size() - 1] == '/')
			ResponseBuilder(this->fullPath_ + "index.html", "200");
		else
			ResponseBuilder(this->fullPath_ + "/index.html", "200");
	}
	else if (_SearchForFile(this->fullPath_))
		ResponseBuilder(ServerConf_.root + request_.request_line.find("target")->second, "200");
	else{
		std::string path = ServerConf_.root;
		if (path[path.size() - 1] == '/')
			ResponseBuilder(path + "errors/404.html", "404");
		else
			ResponseBuilder(path + "/errors/404.html", "404");
	}
}

static void createCGI(const std::map<std::string, std::string> &request_line, const ServerConfig &con,
		  const std::map<std::string, std::string> &headers) {
	CGI cgi = CGI(request_line, con, headers);
}

void Response::PostRequest() {
	this->response_line["status_code"] = "405";
	this->response_line["status"] = GetStatusText(this->response_line.find("status_code")->second);
//	bodyLine
//	std::ifstream fin(path, std::ios::in|std::ios::binary|std::ios::ate);
//	int size;
//	if (fin.is_open())
//	{
//		fin.seekg(0, std::ios::end);
//		size = fin.tellg();
//		char *contents = new char [size];
//		fin.seekg (0, std::ios::beg);
//		fin.read (contents, size);
//		fin.close();
//		std::string str(contents, size);
//		delete [] contents;
//		this->body = str;
//	}
//	headerLine
	this->headers["Content-Type"] = "text/html; charset=utf-8";
	this->headers["Content-Length"] = std::to_string(this->body.size());
}
void Response::HeadRequest() {
	SetStatus("405");
//	bodyLine
//	std::ifstream fin(path, std::ios::in|std::ios::binary|std::ios::ate);
//	int size;
//	if (fin.is_open())
//	{
//		fin.seekg(0, std::ios::end);
//		size = fin.tellg();
//		char *contents = new char [size];
//		fin.seekg (0, std::ios::beg);
//		fin.read (contents, size);
//		fin.close();
//		std::string str(contents, size);
//		delete [] contents;
//		this->body = str;
//	}
//	headerLine
	this->headers["Content-Type"] = "text/html; charset=utf-8";
	this->headers["Content-Length"] = std::to_string(this->body.size());
}

bool Response::CheckMethodCorrectness() {
	std::string methods[] = { "GET", "POST", "HEAD", "DELETE", "PUT" };
	for (int i = 0; i < 5; i++){
		if (methods[i] == request_.request_line.find("method")->second)
			return 1;
	}
	SetStatus("405");
	return 0;
}

bool Response::CheckLocationMethods() {
	for (int i = 0; i < this->location_.http_methods.size(); i++) {
		if (this->location_.http_methods[i] == request_.request_line.find("method")->second)
			return 1;
	}
	SetStatus("405");
	return 0;
}
void Response::CorrectPath()
{
	if (this->ServerConf_.root[this->ServerConf_.root.size() - 1] == '/')
		this->fullPath_ = this->ServerConf_.root.substr(0, this->ServerConf_.root.size() - 1);
	else
		this->fullPath_ = this->ServerConf_.root;
	if (this->location_.root[this->location_.root.size() - 1] == '/')
		this->fullPath_ += this->location_.root.substr(0, this->location_.root.size() - 1);
	else
		this->fullPath_ += this->location_.root;
	if (this->cleanTarget_[this->cleanTarget_.size() - 1] == '/')
		this->fullPath_ += this->cleanTarget_.substr(0, this->cleanTarget_.size() - 1);
	else
		this->fullPath_ += this->cleanTarget_;
	std::cout << this->fullPath_ << std::endl;
}

bool Response::CheckLocationCorrectness() {
	std::string path = request_.request_line.find("target")->second;
	path = path.substr(0, path.find('?'));
	this->cleanTarget_ = path;
	bool flag = true;
	while (flag) {
		if (path == "/")
			flag = false;
		if (path != "/" && path[path.size() - 1] == '/')
			path = path.substr(0, path.size() - 1);
		for (int i = 0; i < ServerConf_.locations.size(); i++) {
			std::string location = ServerConf_.locations[i].path;
			if (location != "/" && location[location.size() - 1] == '/')
				location = location.substr(0, location.size() - 1);
			if (path == location) {
				location_ = ServerConf_.locations[i];
				CorrectPath();
				return 1;
			}
		}
		path = path.substr(0, path.rfind('/') + 1);
	}
	SetStatus("404");
	return 0;
}

void Response::SetStatus(std::string code) {
	this->response_line["status_code"] = code;
	this->response_line["status"] = GetStatusText(code);
}

std::string Response::SetResponseLine() {
	freeResponse();
	HTTPVersionControl();
	if (CheckMethodCorrectness() && CheckLocationCorrectness() && CheckLocationMethods()) {
		DIR *dir = opendir(this->fullPath_.c_str());
		if (location_.autoindex && dir) {
			SetStatus("200");
			_createHTMLAutoIndex(dir);
			closedir(dir);
		} else {
			if(request_.request_line.find("method")->second == "GET" /*|| request_.request_line.find("method")->second == "HEAD"*/) {
				GetRequest();
			}
			else if (request_.request_line.find("method")->second == "POST"){
				PostRequest();
			}
			else if (request_.request_line.find("method")->second == "HEAD") {
				HeadRequest();
			}
			else if (request_.request_line.find("method")->second == "PUT") {
				SetStatus("201");

				this->headers["Content-Type"] = "text/html; charset=utf-8";
				this->headers["Content-Length"] = std::to_string(this->body.size());
			}
		}
	}
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
		if (request_.request_line.find("method")->second != "HEAD") {

			response += this->body;
		}
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










std::string Response::_getTimeModify(std::string path) {
	struct stat file_info;
	if (lstat(path.c_str(), &file_info) != 0) {
		std::cout << "Error: lstat wtf?!" << std::endl;
		exit(1);
	}
	char date[36];
	strftime(date, 36, "%d.%m.%Y %H:%M:%S", localtime(&file_info.st_mtime));
	std::string tmpDate = date;
	return tmpDate + "\n";
}

void Response::_createHTMLAutoIndex(DIR *dir) {
	std::string autoIndexBegin = "<!DOCTYPE html>\n"
								 "<html>\n"
								 "<head>\n"
								 "<title>Index of /</title></head>\n"
								 "<body bgcolor=\"white\">\n"
								 "<h1>Index of /</h1>\n"
								 "<hr>\n"
								 "<pre>\n";
	std::string autoIndexEnd = "</pre>\n"
							   "<hr>\n"
							   "</body>\n"
							   "</html>\n";
	if (dir == nullptr) {
		throw std::runtime_error("Error: cannot open dir (autoindex)");
	}
	std::string concatLink;
	std::string fileName;
	struct dirent *entity;
	this->body = autoIndexBegin;
	while ((entity = readdir(dir))) {
		if (strcmp(entity->d_name, ".") != 0) {
			if (entity->d_type != DT_DIR) {
				fileName = entity->d_name;
			} else {
				fileName = entity->d_name;
				fileName += "/";
			}
			concatLink
					.append("     <a href=\"")
					.append(fileName)
					.append("\">")
					.append(fileName)
					.append("</a>")
					.append(60 - concatLink.length() + fileName.length(), ' ')
					.append(_getTimeModify(this->fullPath_ + "/" + fileName));
			this->body += concatLink;
			fileName.clear();
			concatLink.clear();
		}
	}
	this->body += autoIndexEnd;
	this->headers["Content-Type"] = "text/html; charset=utf-8";
	this->headers["Content-Length"] = std::to_string(this->body.size());
}

bool Response::_SearchForDir() {
	DIR *dr;
	struct dirent *en;
	dr = opendir(this->fullPath_.c_str());
	if (dr) {
			for (int i = 0; i < location_.index.size(); i++) {
			while ((en = readdir(dr)) != NULL) {
				if (strcmp(en->d_name, location_.index[i].c_str()) == 0) {
					closedir(dr);
					return 1;
				}
			}
		}
		closedir(dr);
	}
	return 0;
}

bool Response::_SearchForFile(const std::string &path){
	DIR *dr;
	struct dirent *en;
	std::string str = path.substr(0, path.rfind('/')).c_str();
	dr = opendir(path.substr(0, path.rfind('/')).c_str());
	if (dr){
		while ((en = readdir(dr)) != NULL){
			if (strcmp(en->d_name, path.substr(path.rfind('/') + 1).c_str()) == 0 && !opendir(path.c_str())){
				closedir(dr);
				return 1;
			}
		}
		closedir(dr);
	}
	return 0;
}





