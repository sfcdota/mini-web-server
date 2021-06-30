
#include "Request.hpp"

const std::map<std::string, std::string> &Request::GetRequestLine() const {
  return request_line;
}
void Request::SetRequestLine(const std::map<std::string, std::string> &request_line) {
  Request::request_line = request_line;
}
const std::map<std::string, std::string> &Request::GetHeaders() const {
  return headers;
}
void Request::SetHeaders(const std::map<std::string, std::string> &headers) {
  Request::headers = headers;
}
void Request::SetBody(const std::string &body) {
  Request::body = body;
}
Request::Request() {

}


void Request::PrintRequestLine() {
  std::cout << "%%%%%%%%%%%%%%%%% REQUEST LINE %%%%%%%%%%%%%%%%%" << std::endl;
  for(std::map<std::string, std::string>::iterator it = request_line.begin(); it != request_line.end(); it++)
      std::cout << it->first << "=" << it->second << std::endl;
  std::cout << "%%%%%%%%%%%%%%%%% END OF REQUEST LINE %%%%%%%%%%%%%%%%%" << std::endl;
}
void Request::PrintHeaders() {
  std::cout << "$$$$$$$$$$$$$$$$ HEADERS BLOCK $$$$$$$$$$$$$$$$" << std::endl;
  for(std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
    std::cout << it->first << ":" << it->second << std::endl;
  std::cout << "$$$$$$$$$$$$$$$$ END OF REQUEST LINE $$$$$$$$$$$$$$$$" << std::endl;
}

void Request::PrintBody() {
  std::cout << "#################### BODY ####################" <<std::endl << body
  << "#################### END OF BODY ####################" << std::endl;
}


const std::string &Request::GetBody() const {
  return body;
}
