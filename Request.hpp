
#ifndef _REQUEST_HPP_
#define _REQUEST_HPP_
#include "includes/allowed_library_includes.hpp"

/* with c++ string impossible to use find functions to get tokens
 * because start pos = 0 means it will search entire input string.
 * Unable to use token just with size_t indexes, need to use
 * token substring which means extra compute time ;(
*/
class Request {
 public:
  explicit Request(std::string & request);
  std::string method;
  std::string target;
  std::string version;
  std::string head;
  std::string body;
  std::map<std::string, std::list<std::string> > headers;
  std::string body; //????????
 private:
  std::vector<std::string> allowed_methods;
  void GetHeadBlocks(std::string & request);
  void GetHead(std::string & request);
  void GetBody(std::string & request);
  bool ValidRequest(std::string & request_line);
  bool ValidMethod(std::string & request, size_t & begin, size_t & token);
  bool ValidTarget(std::string & request, size_t & begin, size_t & token);
  bool ValidVersion(std::string & request, size_t & begin, size_t & token);

};
#endif
