#include "Response.hpp"

const std::string Response::PrintLog(const ResponseLoggingOptions &option) const {
  return std::string();
}

Response::Response(const Request &request): AResponse(request), ServerConf_(_request.GetServerConfig()) {
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

Response::~Response() {}


bool Response::SetResponse(const std::string &path, const std::string &status_code) {
	SetStatus(status_code);
	if ((this->_request.GetRequestLine().at("method") == "POST"
		|| this->_request.GetRequestLine().at("method") == "GET")&& !SetBody(path))
		return false;
	SetHeader("Content-Type", path);
	return true;
}

bool Response::HTTPVersionControl() {
	this->response_line["version"] = "HTTP/1.1 ";
	if (_request.GetRequestLine().at("version") != "HTTP/1.1") {
		ErrorHandler("505");
		return false;
	}
	return true;
}
void Response::ErrorHandler(const std::string &status_code) {
	SetStatus(status_code);
	this->body_ = "<!DOCTYPE html>\n"
				"<html lang=\"en\">\n"
				"<head>\n"
				"<meta charset=\"UTF-8\">\n"
				"<title>webserv</title>\n"
				"</head>\n"
				"<body>\n"
				"<center>\n"
				"<h1>" +
				  this->response_line["status_code"] +
				  this->response_line["status"] +
				"</h1>\n" +
				"</center>\n"
				"</body>\n"
				"</html>\n";
	SetHeader("Content-Type", ".html");
}

const std::string & Response::GetBody() const {
    return this->body_;
}

const std::string & Response::GetFullPath() const {
	return this->fullPath_;
}

const location & Response::GetLocation() const {
	return this->location_;
}

const std::string & Response::GetCleanTarget() const {
	return this->cleanTarget_;
}

const std::map<std::string, std::string> & Response::GetHeaders() const {
	return this->headers;
}

const Request & Response::GetRequestClass() const {
	return this->_request;
}

 bool Response::MakeDirectory() {
	 int start = 0;
	 int end = 0;
	 std::string dir;
	 std::string path;
	 if (this->location_.root.size() == 1)
	 	path = this->cleanTarget_;
	 else
	 	path = this->location_.root + this->cleanTarget_;
	
	 if (chdir(ServerConf_.root.c_str()) == -1){
		 ErrorHandler("500");
		 return false;
	 }
	 while (true){
		 if ((end = (path).find('/', end + 1)) != std::string::npos) {
			 dir = (path).substr(start + 1, end - start - 1);
			 if (!dir.size())
			 if (chdir(dir.c_str()) == -1) {
				 if (mkdir(dir.c_str(), 0777) == -1){
					 ErrorHandler("500");
					 return false;
				 }
				 if (chdir(dir.c_str()) == -1){
					 ErrorHandler("500");
					 return true;
				 }
			 }
			 start = end;
		 }
		 else
			 break ;
	 }
	 return 1;
}

bool Response::OpenOrCreateFile() {
	int fd = open((this->fullPath_).c_str(), O_CREAT | O_RDWR | O_TRUNC , 0777);
	if (fd == -1) {
		ErrorHandler("500");
		return false;
	}
	write(fd, this->_request.GetBody().c_str(), this->_request.GetBody().size());
	close(fd);
	return true;
}

void Response::GetRequest()  {
	_SearchForDir();
}


void Response::PostRequest() {
	if (this->_request.IsFailed()) {
		ErrorHandler(std::to_string(this->_request.GetStatusCode()));
	}
	else if (this->_request.GetHeaders().find("Content-Length") == this->_request.GetHeaders().end()){
		ErrorHandler("411");
	}
//	else if (this->_request.GetHeaders().at("Content-Length") == "0"){
//		ErrorHandler("405");
//	}
//	else if (this->response_line["target"].substr(this->response_line["target"].rfind('/')) != "/site"){
//		CGI::executeCGI(_request, *this);
//	}
	else {
		if (!MakeDirectory())
			return ;
		if (!FillBody(this->fullPath_))
			return ;
		if (_SearchForFile(this->fullPath_)){
			OpenOrCreateFile();
//			SetStatus("200");
//			if (!SetBody(this->fullPath_))
//				return ;
//			SetHeader("Content-Type", this->fullPath_);
			if (!SetResponse(this->fullPath_, "200"))
				return ;
		} else {
			OpenOrCreateFile();
//			SetStatus("201");
//			if (!SetBody(this->fullPath_))
//				return ;
//			SetHeader("Content-Type", this->fullPath_);
			if (!SetResponse(this->fullPath_, "201"))
				return ;
			SetHeader("Location", this->_request.GetRequestLine().at("target"));
		}
	}
}
void Response::HeadRequest() {
	ErrorHandler("405");
}



void Response::PutRequest() {
	if (this->_request.GetHeaders().find("Content-Length") == this->_request.GetHeaders().end()){
		ErrorHandler("411");
	}
	else if (this->_request.GetHeaders().at("Content-Length") == "0"){
		ErrorHandler("405");
	} else {
		if (!MakeDirectory())
			return ;
		if (_SearchForFile(this->fullPath_)){
			if (!FillBody(this->fullPath_))
				return ;
			OpenOrCreateFile();
//			SetStatus("200");
//			SetHeader("Content-Type", this->fullPath_);
			if (!SetResponse(this->fullPath_, "200"))
				return ;
		} else {
			OpenOrCreateFile();
//			SetStatus("201");
//			SetHeader("Content-Type", this->fullPath_);
			if (!SetResponse(this->fullPath_, "201"))
				return ;
//			SetHeader("Location", this->re);
		}
	}
	
}

void Response::DeleteRequest() {
	if (_SearchForFile(this->fullPath_)){
		if (std::remove(this->fullPath_.c_str())) {
			ErrorHandler("500");
			return ;
		}
		SetResponse(this->fullPath_, "200");
	} else {
		SetResponse(this->fullPath_, "204");
	}
}

bool Response::CheckMethodCorrectness() {
	std::string methods[] = { "GET", "POST", "HEAD", "DELETE", "PUT" };
	for (int i = 0; i < 5; i++){
		if (methods[i] == this->_request.GetRequestLine().at("method"))
			return true;
	}
	ErrorHandler("405");
	return false;
}

bool Response::CheckLocationMethods() {
	for (int i = 0; i < this->location_.http_methods.size(); i++) {
		if (this->location_.http_methods[i] == _request.GetRequestLine().at("method"))
			return true;
	}
	ErrorHandler("405");
	return false;
}
//void Response::CorrectPath()
//{
//	if (this->ServerConf_.root[this->ServerConf_.root.size() - 1] == '/')
//		this->fullPath_ = this->ServerConf_.root.substr(0, this->ServerConf_.root.size() - 1);
//	else
//		this->fullPath_ = this->ServerConf_.root;
//
//	if (this->location_.root[this->location_.root.size() - 1] == '/')
//		this->fullPath_ += this->location_.root.substr(0, this->location_.root.size() - 1);
//	else
//		this->fullPath_ += this->location_.root;
//
//	if (this->cleanTarget_[this->cleanTarget_.size() - 1] == '/')
//		this->fullPath_ += this->cleanTarget_.substr(0, this->cleanTarget_.size() - 1);
//	else
//		this->fullPath_ += this->cleanTarget_;
//}

std::string Response::PathBuilder(const std::string &path) {
	if (path[path.size() - 1] == '/')
		return path.substr(0, path.size() - 1);
	return path;
}

bool Response::CheckLocationCorrectness() {
	std::string path = this->_request.GetRequestLine().at("target");
	path = path.substr(0, path.find('?'));
	this->cleanTarget_ = path;
	bool flag = true;
	while (flag) {
		if (path == "/")
			flag = false;
		if (path != "/" && path[path.size() - 1] == '/')
			path = path.substr(0, path.size() - 1);
		for (int i = 0; i < this->ServerConf_.locations.size(); i++) {
			std::string location = this->ServerConf_.locations[i].path;
			if (location != "/" && location[location.size() - 1] == '/')
				location = location.substr(0, location.size() - 1);
			if (path == location) {
				this->location_ = this->ServerConf_.locations[i];
//				CorrectPath();
				this->fullPath_ = PathBuilder(ServerConf_.root)
								+ PathBuilder(location_.root)
								+ PathBuilder(cleanTarget_);
				return true;
			}
		}
		path = path.substr(0, path.rfind('/') + 1);
	}
	ErrorHandler("404");
	return false;
}

void Response::SetStatus(const std::string &code) {
	this->response_line["status_code"] = code;
	this->response_line["status"] = GetStatusText(code);
}

bool Response::IsRequestCorrect() {
	return (HTTPVersionControl()
			&& CheckMethodCorrectness()
			&& CheckLocationCorrectness()
			&& CheckLocationMethods());
}

const std::string Response::GetResponse() {
	if (IsRequestCorrect()) {
		if (location_.autoindex) {
          DIR *dir = opendir(this->fullPath_.c_str());
          if (dir) {
            SetStatus("200");
            _createHTMLAutoIndex(dir);
            closedir(dir);
          }
		}
        else if (_request.GetRequestLine().at("target").rfind('.') != std::string::npos &&
              this->_request.GetRequestLine().at("target").substr(_request.GetRequestLine().at("target").rfind('.'))
                == location_.cgi_extension)
      {
          return CGI::executeCGI(this->_request, *this);
      }
		else {
			if(_request.GetRequestLine().at("method") == "GET") {
				GetRequest();
			} else if (_request.GetRequestLine().at("method") == "POST"){
				PostRequest();
			} else if (_request.GetRequestLine().at("method") == "HEAD") {
				HeadRequest();
			} else if (_request.GetRequestLine().at("method") == "PUT") {
				PutRequest();
			} else if (this->_request.GetRequestLine().at("method") == "DELETE")
				DeleteRequest();
		}
	}
	return SendResponse();
}

const std::string Response::SendResponse() {
	std::string response;
	std::map<std::string, std::string>::iterator begin;

	SetHeader("Date", GetTimeGMT());
	SetHeader("Server", "webserv");
	SetHeader("Content-Length", std::to_string(this->body_.size()));

	response = this->response_line["version"];
	response += this->response_line["status_code"];
	response += this->response_line["status"];
	for (begin = this->headers.begin(); begin != this->headers.end(); begin++) {
		if (begin == this->headers.begin())
			response += "\r\n";
		response += begin->first + ": ";
		response += begin->second + "\r\n";
	}
	response += "\r\n";
	if (_request.GetRequestLine().at("method") != "HEAD") {
		response += this->body_;
	}
	return response;
}



std::string Response::GetStatusText(std::string code) {
	std::map<std::string, std::string> status_text_;
	status_text_["100"] = " Continue";
	status_text_["101"] = " Switching Protocols";
	status_text_["102"] = " Processing";
	status_text_["103"] = " Early Hints";
	status_text_["200"] = " OK";
	status_text_["201"] = " Created";
	status_text_["202"] = " Accepted";
	status_text_["203"] = " Non_Authoritative Information";
	status_text_["204"] = " No Content";
	status_text_["205"] = " Reset Content";
	status_text_["300"] = " Multiple Choices";
	status_text_["301"] = " Moved Permanently";
	status_text_["302"] = " Found";
	status_text_["303"] = " See Other";
	status_text_["305"] = " Use Proxy";
	status_text_["306"] = " (Unused)";
	status_text_["307"] = " Temporary Redirect";
	status_text_["400"] = " Bad Request";
	status_text_["402"] = " Payment Required";
	status_text_["403"] = " Forbidden";
	status_text_["404"] = " Not Found";
	status_text_["405"] = " Method Not Allowed";
	status_text_["406"] = " Not Acceptable";
	status_text_["408"] = " Request Timeout";
	status_text_["409"] = " Conflict";
	status_text_["410"] = " Gone";
	status_text_["411"] = " Length Required";
	status_text_["413"] = " Payload Too Large";
	status_text_["414"] = " URI Too Long";
	status_text_["415"] = " Unsupported Media Type";
	status_text_["417"] = " Expectation Failed";
	status_text_["426"] = " Upgrade Required";
	status_text_["500"] = " Internal Server Error";
	status_text_["501"] = " Not Implemented";
	status_text_["502"] = " Bad Gateway";
	status_text_["503"] = " Service Unavailable";
	status_text_["504"] = " Gateway Timeout";
	status_text_["505"] = " HTTP Version Not Supported";
	return status_text_.find(code)->second;
}










const std::string Response::_getTimeModify(const std::string &path) {
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
	this->body_ = autoIndexBegin;
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
			this->body_ += concatLink;
			fileName.clear();
			concatLink.clear();
		}
	}
	this->body_ += autoIndexEnd;
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
	if (key == "Content-Type" && value.rfind('.') == std::string::npos)
		this->headers[key] = "text/plane";
	else if (key == "Content-Type" && (value.empty() || this->content_type_.find(value.substr(value.rfind('.'))) == this->content_type_.end()))
		this->headers[key] = "text/plane";
	else if (key == "Content-Type")
		this->headers[key] = this->content_type_.find(value.substr(value.rfind('.')))->second;
	else
		this->headers[key] = value;
}

//void Response::SetHeaders() {
//	if(this->response_line.find("method")->second == "GET")
//		if (!this->headers.find("Content-Type")->second.size())
//			this->headers["Content-Type"] = this->content_type_.find(".html")->second;
//	SetHeader("Content-Length", std::to_string(this->body.size()));
//	SetHeader("Date", GetTimeGMT());
//}

bool Response::_SearchForDir() {
	DIR *dr;
	struct dirent *en;
	dr = opendir(this->fullPath_.c_str());
	if (dr) {
			for (int i = 0; i < location_.index.size(); i++) {
			while ((en = readdir(dr)) != NULL) {
				if (strcmp(en->d_name, location_.index[i].c_str()) == 0) {
					closedir(dr);
					if (!SetResponse(this->fullPath_ + "/" + location_.index[i], "200"))
						return 0;
					return 1;
				}
			}
		}
		closedir(dr);
	}
	else if (_SearchForFile(this->fullPath_)) {
		if (!SetResponse(this->fullPath_, "200"))
			return 0;
		return 1;
	}
	ErrorHandler("404");
//	SetErrorResponse("404");
	return 0;
}

bool Response::_SearchForFile(const std::string &path){
	DIR *dr;
	struct dirent *en;
	std::string str = path.substr(0, path.rfind('/')).c_str();
	dr = opendir(path.substr(0, path.rfind('/')).c_str());
	if (dr){
		while ((en = readdir(dr)) != NULL) {
          DIR *hui = opendir(path.c_str());
          if (strcmp(en->d_name, path.substr(path.rfind('/') + 1).c_str()) == 0 && !hui) {
            closedir(dr);
            return 1;
          }
          if (hui)
            closedir(hui);
        }
		closedir(dr);
	}
	return 0;
}

bool Response::FillBody(const std::string &path) {
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
		if (this->_request.GetRequestLine().at("method") == "POST" && _request.GetBody().size() == str.size() && this->_request.GetBody() == str){
			SetStatus("303");
			SetHeader("Location", this->fullPath_);
			fin.close();
			return 0;
		}
      fin.close();
    } else {
		ErrorHandler("500");
		return 0;
	}
	return 1;
}

bool Response::SetBody(const std::string &path) {
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
		this->body_ = str;
	}
	else {
		ErrorHandler("500");
		return 0;
	}
	return 1;
}