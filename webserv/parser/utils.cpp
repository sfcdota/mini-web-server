#include "parser.hpp"

int to_int(std::string str){
	int n = 0;
	for (int i = 0; i < str.size(); i++) {
		if (n > 65535)
			return -1;
		if (str[i] >= '0' && str[i] <= '9')
			n = n * 10 + (str[i] - 48);
		else
			return -1;
	}
	return n;
}

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

std::string getTimeModify(std::string path) {
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

void	createHTMLAutoIndex(ServerConfig &con, std::string linkPath) {
	DIR *dir = opendir((con.root + linkPath).c_str());
	std::ofstream fout;
	std::string path = con.root + "/autoindex/autoindex.html";
	fout.open(path);
	if (dir == nullptr || !fout.is_open()) {
		std::cout << "Error: cannot open dir (createAutoIndexHTML)" << std::endl;
		exit(1);
	}
	std::string concatLink;
	std::string fileName;
	struct dirent *entity;
	fout << autoIndexBegin;
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
				.append(getTimeModify(con.root + "/" + fileName));
			fout << concatLink;
			fileName.clear();
			concatLink.clear();
		}
	}
	fout << autoIndexEnd;
	closedir(dir);
	fout.close();
}

bool SearchForDir(const std::string &path) {
	DIR *dr;
	struct dirent *en;
	dr = opendir(path.c_str());
	if (dr) {
		while ((en = readdir(dr)) != NULL) {
			if(strcmp(en->d_name, "index.html") == 0) {
				closedir(dr);
				return 1;
			}
		}
		closedir(dr);
	}
	return 0;
}

bool SearchForFile(const std::string &path){
	DIR *dr;
	struct dirent *en;
	dr = opendir(path.substr(0, path.rfind('/') + 1).c_str());
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

std::string rootDir(){
	std::array<char, 128> buffer;
	char *cmd = "/bin/pwd";
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}

void errors(std::string str){
	std::cout << str << std::endl;
	exit(1);
}

void clear_loc(parsConfig &con){
	con.location.autoindex = false;
	con.location.upload_path = "";
	con.location.http_methods.clear();
	con.location.cgi_extension = "";
	con.location.cgi_path = "";
	con.location.index.clear();
	con.location.root = "";
	con.location.retur.clear();
}

void init_location(parsConfig &con){
	con.location_body["autoindex"] = 0;
	con.location_body["limit_except"] = 0;
	con.location_body["root"] = 0;
	con.location_body["index"] = 0;
	con.location_body["cgi_extension"] = 0;
	con.location_body["cgi_path"] = 0;
	con.location_body["upload_path"] = 0;
	con.location_body["return"] = 0;
	con.pars.location_status = 0;
}

void init_server(parsConfig &con){
	con.server_body["listen"] = 0;
	con.server_body["server_name"] = 0;
	con.server_body["error_page"] = 0;
	con.server_body["client_max_body_size"] = 0;
	con.server_body["return"] = 0;
	con.pars.server_status = 0;
}

void isstring(std::vector<std::string> &bla, std::string &tmp){
	if (tmp.size()){
		bla.push_back(tmp);
		tmp = "";
	}
}

void word_spliter(char *line, std::vector<std::string> &bla){
	std::string tmp;

	while (*line){
		if (isspace(*line)) {
			isstring(bla, tmp);
			line++;
			continue;
		}
		if (*line == COMMENT) {
			isstring(bla, tmp);
			break;
		}
		if (*line == '{' || *line == '}' || *line == ';'){
			isstring(bla, tmp);
			tmp.push_back(*line);
			bla.push_back(tmp);
			tmp = "";
			line++;
			continue;
		}
		tmp += *line;
		line++;
	}
	isstring(bla, tmp);
}