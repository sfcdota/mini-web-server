
#ifndef REQUEST_HPP_
#define REQUEST_HPP_
#include "includes/allowed_library_includes.hpp"
#include "includes/simplifiers.hpp"

/* with c++ string impossible to use find functions to get tokens
 * because start pos = 0 means it will search entire input string.
 * Unable to use token just with size_t indexes, need to use
 * token substring which means extra compute time ;(
*/
class Request {
 public:
  explicit Request(std::string & request);
  std::map<std::string, std::string> request_line;
  std::map<std::string, std::list<std::string> > headers;
  std::string body; //????????
  size_t status_code;
  bool failed;
 private:
  std::vector<std::string> allowed_methods;
  std::map<std::string, std::string> & GetRequestLine(string & request);
  bool ValidParsedHeaders(std::string & request);
  bool ValidParsedRequest(std::string & request);
  bool ValidMethod(std::string & method);
  bool ValidTarget(std::string & target);
  bool ValidVersion(std::string & version);
  size_t left;
  size_t right;
};
#endif // REQUEST_HPP_
