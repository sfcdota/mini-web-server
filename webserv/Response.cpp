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
//	this->content_type_[] = "application/x-executable";
//	this->content_type_[] = "application/graphql";
	this->content_type_[".js"] = "application/javascript";
	this->content_type_[".json"] = "application/json";
//	this->content_type_[] = "application/ld+json";
	this->content_type_[".doc"] = "application/msword (.doc)";
	this->content_type_[".pdf"] = "application/pdf";
	this->content_type_[".sql"] = "application/sql";
//	this->content_type_[] = "application/vnd.api+json";
	this->content_type_[".xls"] = "application/vnd.ms-excel (.xls)";
	this->content_type_[".ppt"] = "application/vnd.ms-powerpoint (.ppt)";
	this->content_type_[".odt"] = "application/vnd.oasis.opendocument.text (.odt)";
	this->content_type_[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation (.pptx)";
	this->content_type_[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet (.xlsx)";
	this->content_type_[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document (.docx)";
//	this->content_type_[] = "application/x-www-form-urlencoded";
	this->content_type_[".xml"] = "application/xml";
	this->content_type_[".zip"] = "application/zip";
	this->content_type_[".zst"] = "application/zstd (.zst)";
	this->content_type_[".bin"] = "application/macbinary (.bin)";
//	this->content_type_[] = "audio/mpeg";
//	this->content_type_[] = "audio/ogg";
//	this->content_type_[] = "image/apng";
//	this->content_type_[] = "image/avif";
//	this->content_type_[] = "image/flif";
//	this->content_type_[] = "image/gif";
	this->content_type_[".jpg"] = "image/jpeg (.jpg, .jpeg, .jfif, .pjpeg, .pjp) [11]";
	this->content_type_[".jpeg"] = "image/jpeg";
	this->content_type_[".jfif"] = "image/jpeg";
	this->content_type_[".pjpeg"] = "image/jpeg";
	this->content_type_[".pjp"] = "image/jpeg";
//	this->content_type_[] = "image/jxl";
	this->content_type_[".png"] = "image/png";
	this->content_type_[".svg"] = "image/svg+xml (.svg)";
//	this->content_type_[] = "image/webp";
//	this->content_type_[] = "image/x-mng";
//	this->content_type_[] = "multipart/form-data";
	this->content_type_[".ico"] = "image/x-icon";
	this->content_type_[".css"] = "text/css";
	this->content_type_[".csv"] = "text/csv";
	this->content_type_[".html"] = "text/html";
	this->content_type_[".php"] = "text/php";
//	this->content_type_[] = "text/plain";
	this->content_type_[".xml"] = "text/xml";

}


void Response::ResponseBuilder(const std::string &path, const std::string &status_code) {
	SetStatus(status_code);
	SetBody(path);
//	if (path.empty() || !(this->content_type_.find(path.substr(path.find('.')))->second.size() < 1))
//		this->headers["Content-Type"] = "text/plane";
//	else
//		this->headers["Content-Type"] = this->content_type_.find(path.substr(path.find('.')))->second;
	if (path.size() > 0)
		SetHeader("Content-Type", this->content_type_.find(path.substr(path.find('.')))->second);
	SetHeader("Content-Type", "non_exist");
}

bool Response::HTTPVersionControl() {
	if (request_.request_line.find("version")->second == "HTTP/1.1") {
		this->response_line["version"] = "HTTP/1.1";
		return 1;
	}
	SetStatus("505");
	SetBody(ServerConf_.root + "/errors/505.html");
	SetHeader("Content-Type", ".html");
	return 0;
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
	this->headers["Content-Type"] = this->content_type_.find(".html")->second;
	this->headers["Content-Length"] = std::to_string(this->body.size());
}
void Response::HeadRequest() {
	SetStatus("405");
	SetHeaders();
}

void Response::PutRequest() {
	int fd = open((this->ServerConf_.root + "/bin" + this->fullPath_.substr(this->fullPath_.rfind('/'))).c_str(), O_CREAT | O_RDWR | O_TRUNC , 0777);
	if (fd == -1) {
		throw std::runtime_error("Error: cannot open create/open file");
	}
	
	write(fd, this->request_.body.c_str(), this->request_.body.size());
	close(fd);
	ResponseBuilder(this->fullPath_.substr(this->fullPath_.rfind('/')), "201");
	
}

bool Response::CheckMethodCorrectness() {
	std::string methods[] = { "GET", "POST", "HEAD", "DELETE", "PUT" };
	for (int i = 0; i < 5; i++){
		if (methods[i] == request_.request_line.find("method")->second)
			return 1;
	}
	SetStatus("405");
	SetBody(ServerConf_.root + "/errors/405.html");
	SetHeader("Content-Type", ".html");
	return 0;
}

bool Response::CheckLocationMethods() {
	for (int i = 0; i < this->location_.http_methods.size(); i++) {
		if (this->location_.http_methods[i] == request_.request_line.find("method")->second)
			return 1;
	}
	SetStatus("405");
	SetBody(ServerConf_.root + "/errors/405.html");
	SetHeader("Content-Type", ".html");
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
	SetBody(ServerConf_.root + "/errors/404.html");
	SetHeader("Content-Type", ".html");
	return 0;
}

void Response::SetStatus(std::string code) {
	this->response_line["status_code"] = code;
	this->response_line["status"] = GetStatusText(code);
}

std::string Response::SetResponseLine() {
//	freeResponse();
	if (HTTPVersionControl() && CheckMethodCorrectness() && CheckLocationCorrectness() && CheckLocationMethods()) {
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
//				SetStatus("201");
//				SetHeaders();
				PutRequest();
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

	SetHeader("Date", GetTimeGMT());
	SetHeader("Server", "webserv");
	SetHeader("Content-Length", std::to_string(this->body.size()));

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
	SetHeader("Content-Type", ".html");
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

void Response::SetHeader(const std::string &key, const std::string &value) {
	if (key == "Content-Type" && (!value.empty() || this->content_type_.find(value)->second.size() < 1))
		this->headers[key] = "text/plane";
	else if (key == "Content-Type")
		this->headers[key] = this->content_type_.find(value)->second;
	else
		this->headers[key] = value;
}

void Response::SetHeaders() {
	if(this->response_line.find("method")->second == "GET")
		if (!this->headers.find("Content-Type")->second.size())
			this->headers["Content-Type"] = this->content_type_.find(".html")->second;
//	this->headers["Content-Length"] = std::to_string(this->body.size());
	SetHeader("Content-Length", std::to_string(this->body.size()));
	//	this->headers["Date"] = GetTimeGMT();
	SetHeader("Date", GetTimeGMT());
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
		ResponseBuilder(this->fullPath_, "200");
		return 1;
	}
	SetErrorResponse("404");
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

void Response::SetBody(const std::string &path) {
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
//	else {
//		SetStatus("500");
//		SetHeaders();
//	}
}

void Response::SetErrorResponse(std::string status_code) {
	std::vector<error> tmpVec = ServerConf_.error_pages;
	for (int index = 0; index < tmpVec.size(); index++) {
		if (status_code == std::to_string(tmpVec[index].error_code)) {
			ResponseBuilder(ServerConf_.root + tmpVec[index].error_path, status_code);
			return;
		}
	}
	ResponseBuilder(ServerConf_.root + "/errors/" + this->headers["status_code"] + ".html", status_code);
};