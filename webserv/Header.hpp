
#ifndef WEBSERV_HEADER_HPP_
#define WEBSERV_HEADER_HPP_
#include <string>
#include <ctime>
//Full public class ?


struct Header {
 public:
  std::string accept_charsets;
  std::string accept_language;
  std::string allow;
  std::string authorization;
  std::string content_language;
  std::string content_length;
  std::string content_location;
  std::string content_type;
  time_t date;
  std::string host;
  std::string last_modified;
  std::string location;
  std::string referer;
  std::string retry_after;
  std::string server;
  std::string transfer_encoding;
  std::string user_agent;
  std::string www_authenticate;
};

//struct Request {
//  std::string method;
//  std::string target;
//  std::string http_version;
//  Header header;
//  std::string body; //?
//};
#endif // WEBSERV_HEADER_HPP_