#include "parser.hpp"

void parsLocation(parsConfig &con, int &i){
  std::string tmp;
	for (; i < con.vec.size(); i++){
	  if (con.vec[i] == "root" && !con.location_body["root"]){
		  con.location_body["root"] = 1;
		  if (con.vec[i + 1] != ";")
			  con.location.root = con.vec[i + 1];
		  else
			  errors("location root error!");
		  if (con.vec[i + 2] != ";")
			  errors("location root error!");
		  i += 2;
	  }
	  else if (con.vec[i] == "autoindex" && !con.location_body["autoindex"]){
	    con.location_body["autoindex"] = 1;
		if (con.vec[i + 1] == "on")
		  con.location.autoindex = true;
		else if (con.vec[i + 1] == "off")
		  con.location.autoindex = false;
		else
			errors("location autoindex error!");
		if (con.vec[i + 2] != ";")
			errors("location autoindex error!");
		i += 2;
	  }
	  else if (con.vec[i] == "index" && !con.location_body["index"]){
		while (++i < con.vec.size(), con.vec[i] != ";"){
		  con.location.index.push_back(con.vec[i]);
		  if (!con.location_body["index"])
		  con.location_body["index"] = 1;
		}
		if (i == con.vec.size() || !con.location_body["index"])
			errors("location index error1!");
	  }
	  else if (con.vec[i] == "limit_except" && !con.location_body["limit_except"]){
		while (++i < con.vec.size(), con.vec[i] != ";"){
		  if (con.vec[i] == "POST" || con.vec[i] == "PUT" || con.vec[i] == "DELETE" || con.vec[i] == "GET" || con.vec[i] == "HEAD")
			  con.location.http_methods.push_back(con.vec[i]);
		  else
			  errors("locaiton limit_except error!");
		  if (!con.location_body["index"])
			con.location_body["index"] = 1;
		}
		if (i == con.vec.size() || !con.location_body["index"])
			errors("location index error2!");
	  }
	  else if (con.vec[i] == "upload_path" && !con.location_body["upload_path"]){
		con.location_body["upload_path"] = 1;
		if (con.vec[i + 1] != ";")
		  con.location.upload_path = con.vec[i + 1];
		else
			errors("location upload_path error!");
		if (con.vec[i + 2] != ";")
			errors("location upload_path error!");
		i += 2;
	  }
	  else if (con.vec[i] == "cgi_extension" && !con.location_body["cgi_extension"]){
		con.location_body["cgi_extension"] = 1;
		if (con.vec[i + 1] != ";")
		  con.location.cgi_extension = con.vec[i + 1];
		else
			errors("location cgi_extension error!");
		if (con.vec[i + 2] != ";")
			errors("location cgi_extension error!");
		i += 2;
	  }
	  else if (con.vec[i] == "cgi_path" && !con.location_body["cgi_path"]){
		con.location_body["cgi_path"] = 1;
		if (con.vec[i + 1] != ";")
		  con.location.cgi_path = con.vec[i + 1];
		else
			errors("location cgi_path error!");
		if (con.vec[i + 2] != ";")
			errors("location cg_path error!");
		i += 2;
	  }
	  else if (con.vec[i] == "return" && !con.location_body["return"]){
	  	int n = 0;
		  con.location_body["return"] = 1;
		  for (int j = 1; i + j < con.vec.size() && con.vec[i + j] != ";"; j++)
		  	n++;
		  if (n == 2) {
			  n = to_int(con.vec[++i]);
			  if (n == -1)
				  errors("location wrong return code!");
			  con.location.retur.push_back(con.vec[i]);
			  con.location.retur.push_back(con.vec[++i]);
		  }
		  else if (n == 1)
			  con.location.retur.push_back(con.vec[++i]);
		  else
			  errors("location return error!");
		  if (i + 1 == con.vec.size())
			  errors("server sintax error!");
		  i++;
	  }
	  else if (con.vec[i] == "}") {
		  con.pars.location_status = 0;
		  con.serv.locations.push_back(con.location);
		  init_location(con);
		  clear_loc(con);
	  }
	  else
		  errors("location error!");
	}
}

void parsServer(parsConfig &con, int &i){
	std::string tmp;
	con.serv.root = rootDir() + "/webserv/site";
//for now con.serv.root equal to "User/ljerrica/webserv/site";
//	con.serv.root = "/Users/ljerrica/Desktop/webserv/webserv/site";
  for(; i < con.vec.size(); i++){
	if (con.vec[i] == "location")
	  con.pars.location_status = 1;
	else if (con.vec[i] != "{" && con.pars.location_status == 1) {
	  con.location.path = con.vec[i];
	  con.pars.location_status = 2;
	}
	else if (con.vec[i] == "{" && con.pars.location_status == 2)
	  con.pars.location_status = 3;
	else if (con.pars.location_status == 3)
		parsLocation(con, i);
	else if (con.vec[i] == "listen" && !con.server_body["listen"]){
	  con.server_body["listen"] = 1;
	  while (++i < con.vec.size() && con.vec[i] != ";"){
		int delimeter = con.vec[i].find(':');
		if (delimeter == -1)
			errors("server listen error!");
//		con.serv.host = con.vec[i].substr(0, delimeter);
        con.serv.host = 2130706433;
		con.serv.port = to_int(con.vec[i].substr(delimeter + 1, con.vec[i].size() - 1 - delimeter));
		if (con.serv.port == -1)
			errors("server listen port error!");
	  }
		if (i == con.vec.size())
			errors("server listen error!");
	}
	else if (con.vec[i] == "server_name" && !con.server_body["server_name"]){
	  con.server_body["server_name"] = 1;
	  if (con.vec[i + 1] != ";")
	    con.serv.server_names.push_back(con.vec[i + 1]);
	  if (con.vec[i + 2] != ";")
		  errors("server: server_name error!");
	  i += 2;
	}
	else if (con.vec[i] == "error_page" && !con.server_body["error_page"]){
	  struct error err;
	  con.server_body["error_page"] = 1;
	  if (++i < con.vec.size() && con.vec[i] != ";")
		  err.error_code = to_int(con.vec[i++]);
	  if (err.error_code < 400 || err.error_code >= 500)
		  errors("server wrong error code!");
	  if (i < con.vec.size() && con.vec[i] != ";")
		err.error_path = con.vec[i++];
	  else
		  errors("server error_page error!");
	  if (i == con.vec.size() || con.vec[i] != ";")
		  errors("server error_page error!");
	  con.serv.error_pages.push_back(err);
	}
	else if (con.vec[i] == "client_max_body_size" && !con.server_body["client_max_body_size"]){
	  con.server_body["client_max_body_size"] = 1;
	  if (++i < con.vec.size())
		con.serv.client_max_body_size = to_int(con.vec[i++]) * 1024 * 1024;
	  if (con.serv.client_max_body_size == -1)
		  errors("server client_max_body_size error!");
	  if (i == con.vec.size() || con.vec[i] != ";")
		  errors("server client_max_body_size error!");
	}
	else if (con.vec[i] == "return" && !con.location_body["return"]){
		int n = 0;
		con.server_body["return"] = 1;
		for (int j = 1; i + j < con.vec.size() && con.vec[i + j] != ";"; j++)
			n++;
		if (n == 2) {
			n = to_int(con.vec[++i]);
			if (n == -1)
				errors("server wrong return code!");
			con.serv.retur.push_back(con.vec[i]);
			con.serv.retur.push_back(con.vec[++i]);
		}
		else if (n == 1)
			con.serv.retur.push_back(con.vec[++i]);
		else
			errors("server return error!");
		if (i + 1 == con.vec.size())
			errors("server sintax error!");
		i++;
	}
	else if (con.vec[i] == "}")
	    con.pars.server_status = false;
	else
		errors("server body error!");
  }
}

ServerConfig	parsConf() {
	parsConfig con;
	char *line;

	con.pars.fd = open("../webserv/configs/server.conf", O_RDONLY);

	init_location(con);
	init_server(con);
	while ((con.pars.res = get_next_line(con.pars.fd, &line)) > -1) {
		word_spliter(line, con.vec);
		for (int i = 0; i < con.vec.size(); i++)
		  if (con.vec[i] == "server")
				con.pars.server_status = 1;
		  else if (con.vec[i] == "{" && con.pars.server_status == 1)
			  con.pars.server_status = 2;
		  else if (con.pars.server_status == 2)
			  parsServer(con, i);
		  else
			  errors("server body error");
		con.vec.clear();
		if (!con.pars.res)
		  break;
  }

  return con.serv;
}

//int main (){
//	ServerConfig s;
//	s = parsConf();
//
//	return 0;
//}