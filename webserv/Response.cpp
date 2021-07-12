#include "Response.hpp"

Response::Response(Request & request): request_(request), ServerConf_(request_.server_config) {
//	Status code explanation
	this->status_text_["100"] = "Continue";
	this->status_text_["101"] = "Switching Protocols";
	this->status_text_["102"] = "Processing";
	this->status_text_["103"] = "Early Hints";
	this->status_text_["200"] = "OK";
	this->status_text_["201"] = "Created";
	this->status_text_["202"] = "Accepted";
	this->status_text_["203"] = "Non_Authoritative Information";
	this->status_text_["204"] = "No Content";
	this->status_text_["205"] = "Reset Content";
	this->status_text_["300"] = "Multiple Choices";
	this->status_text_["301"] = "Moved Permanently";
	this->status_text_["302"] = "Found";
	this->status_text_["303"] = "See Other";
	this->status_text_["305"] = "Use Proxy";
	this->status_text_["306"] = "(Unused)";
	this->status_text_["307"] = "Temporary Redirect";
	this->status_text_["400"] = "Bad Request";
	this->status_text_["402"] = "Payment Required";
	this->status_text_["403"] = "Forbidden";
	this->status_text_["404"] = "Not Found";
	this->status_text_["405"] = "Method Not Allowed";
	this->status_text_["406"] = "Not Acceptable";
	this->status_text_["408"] = "Request Timeout";
	this->status_text_["409"] = "Conflict";
	this->status_text_["410"] = "Gone";
	this->status_text_["411"] = "Length Required";
	this->status_text_["413"] = "Payload Too Large";
	this->status_text_["414"] = "URI Too Long";
	this->status_text_["415"] = "Unsupported Media Type";
	this->status_text_["417"] = "Expectation Failed";
	this->status_text_["426"] = "Upgrade Required";
	this->status_text_["500"] = "Internal Server Error";
	this->status_text_["501"] = "Not Implemented";
	this->status_text_["502"] = "Bad Gateway";
	this->status_text_["503"] = "Service Unavailable";
	this->status_text_["504"] = "Gateway Timeout";
	this->status_text_["505"] = "HTTP Version Not Supported";
//	Content type explanation

}


void Response::ResponseBuilder(const std::string &path, const std::string &status_code) {
//	responseLine
	SetStatus(status_code);
//	bodyLine
//	if (status_code > 399) {
//		path = ServerConf_.root + "/error/" + status_code + ".html";
//	}
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
	SetHeaders();
//	this->headers["Content-Length"] = std::to_string(this->body.size());
}

void Response::HTTPVersionControl() {
	if (request_.request_line.find("version")->second == "HTTP/1.1") {
		this->response_line["version"] = "HTTP/1.1";
	} else {
//		SetStatus();
//		SetHeaders();
		std::cout << "HTTP version error!\n";
		exit(1);
	}
}

void Response::GetRequest() {
	_SearchForDir();
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
	SetHeaders();
}

bool Response::CheckMethodCorrectness() {
	std::string methods[] = { "GET", "POST", "HEAD", "DELETE", "PUT" };
	for (int i = 0; i < 5; i++){
		if (methods[i] == request_.request_line.find("method")->second)
			return 1;
	}
	SetStatus("405");
	SetHeaders();
	return 0;
}

bool Response::CheckLocationMethods() {
	for (int i = 0; i < this->location_.http_methods.size(); i++) {
		if (this->location_.http_methods[i] == request_.request_line.find("method")->second)
			return 1;
	}
	SetStatus("405");
	SetHeaders();
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
	SetHeaders();
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
			if(request_.request_line.find("method")->second == "GET") {
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
				SetHeaders();
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
	std::map<std::string, std::string> status_text_;
	status_text_["100"] = "Continue";
	status_text_["101"] = "Switching Protocols";
	status_text_["102"] = "Processing";
	status_text_["103"] = "Early Hints";
	status_text_["200"] = "OK";
	status_text_["201"] = "Created";
	status_text_["202"] = "Accepted";
	status_text_["203"] = "Non_Authoritative Information";
	status_text_["204"] = "No Content";
	status_text_["205"] = "Reset Content";
	status_text_["300"] = "Multiple Choices";
	status_text_["301"] = "Moved Permanently";
	status_text_["302"] = "Found";
	status_text_["303"] = "See Other";
	status_text_["305"] = "Use Proxy";
	status_text_["306"] = "(Unused)";
	status_text_["307"] = "Temporary Redirect";
	status_text_["400"] = "Bad Request";
	status_text_["402"] = "Payment Required";
	status_text_["403"] = "Forbidden";
	status_text_["404"] = "Not Found";
	status_text_["405"] = "Method Not Allowed";
	status_text_["406"] = "Not Acceptable";
	status_text_["408"] = "Request Timeout";
	status_text_["409"] = "Conflict";
	status_text_["410"] = "Gone";
	status_text_["411"] = "Length Required";
	status_text_["413"] = "Payload Too Large";
	status_text_["414"] = "URI Too Long";
	status_text_["415"] = "Unsupported Media Type";
	status_text_["417"] = "Expectation Failed";
	status_text_["426"] = "Upgrade Required";
	status_text_["500"] = "Internal Server Error";
	status_text_["501"] = "Not Implemented";
	status_text_["502"] = "Bad Gateway";
	status_text_["503"] = "Service Unavailable";
	status_text_["504"] = "Gateway Timeout";
	status_text_["505"] = "HTTP Version Not Supported";
	if (status_text_.find(code) != status_text_.end())
		return status_text_.find(code)->second;
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

static std::string getNumber(int num) {
	if (num < 10) {
		return "0" + std::to_string(num);
	}
	return std::to_string(num);
}

std::string Response::GetTimeGMT() {
	std::stringstream ss;
	time_t t = time(NULL);
	struct tm *gm = gmtime(&t);
	std::string month[] = {"Jun", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	std::string days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	for (int index = 0; index < 7; index++) {
		if (index == gm->tm_wday) {
			ss << days[index] << ", " << gm->tm_mday << " ";
		}
	}
	for (int index = 0; index < 12; index++) {
		if (index == gm->tm_mon) {
			ss << month[index] << " " << (gm->tm_year + 1900) << " " <<
			getNumber(gm->tm_hour) << ":" << getNumber(gm->tm_min) << ":" << getNumber(gm->tm_sec) << " GMT";
		}
	}
	return ss.str();
}

void Response::SetHeaders() {
	this->headers["Content-Type"] = "text/html; charset=utf-8";
	this->headers["Content-Length"] = std::to_string(this->body.size());
	this->headers["Date"] = GetTimeGMT();
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
					ResponseBuilder(this->fullPath_ + "/" + location_.index[i], "200");
					return 1;
				}
			}
		}
		closedir(dr);
	}
	else if (_SearchForFile(this->fullPath_)) {
		ResponseBuilder(ServerConf_.root + request_.request_line.find("target")->second, "200");
		return 1;
	}
	SetStatus("404");
	SetHeaders();
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





