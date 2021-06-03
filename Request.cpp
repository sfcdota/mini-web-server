
#include "Request.hpp"

bool valid_method(size_t begin, size_t end, std::string & request_line) {
  for(std::vector<std::string>::iterator it = allowed_methods.begin(); it != allowed_methods.end(); it++) {
    if (!request_line.compare(begin, end - begin, *it))
      return true;
  }
  return false;
}


bool Request::ValidRequest(std::string &request) {
  method = request.substr(begin, token);
  if (!ValidMethod(request, begin, token))
    return false;
  if (!ValidTarget(request, begin, token))
    return false;
  if (!ValidVersion(request, begin, token))
    return false;
  return false;
}
void Request::GetHeadBlocks(std::string &request) {
  std::list<std::string> request_block;
  size_t begin = 0;
  size_t token;
  while ((token = request.find("\r\n", begin)) != std::string::npos) {
    request_block.push_back(request.substr(begin, token));
    begin = token + 2;
  }
}
void Request::GetHead(std::string &request) {
  size_t begin;
  size_t end;
  if ((end = request.find("\r\n\r\n")) != std::string::npos)
    head = request.substr(0, end + 4);
  else
    head = nullptr;
  end += 4;
  begin = end;

}
void Request::GetBody(std::string &request) {
  size_t end;
  if ((end = request.find("\r\n\r\n", head.length())) != std::string::npos)
    body = request.substr(head.length(), end);
  else
    body = nullptr;
}
Request::Request(std::string &request)  {
  allowed_methods.push_back("GET");
  allowed_methods.push_back("POST");
  allowed_methods.push_back("DELETE");

}
bool Request::ValidMethod(std::string & request, size_t & begin, size_t & token) {
  for (std::vector<std::string>::iterator it = allowed_methods.begin(); it != allowed_methods.end(); it++) {
    if (!request.compare(begin, token, *it)) {
      method = *it;
      begin = token++;
      return true;
    }
  }
  return false;
}

bool Request::ValidTarget(std::string &request, size_t &begin, size_t &token) {
  if (isspace(request[++token])) // origin-form (relative uri) or [absolute-form (with schema)]
    return false;
  return false;
}
bool Request::ValidVersion(std::string &request, size_t &begin, size_t &token) {
  return false;
}
