
#ifndef HEADER_HPP_
#define HEADER_HPP_
#include <string>
#include <ctime>
#include "includes/simplifiers.hpp"
//Full public class ?


struct Header {
 public:
  str accept_charsets;
  str accept_language;
  str allow;
  str authorization;
  str content_language;
  str content_length;
  str content_location;
  str content_type;
  time_t date;
  str host;
  str last_modified;
  str location;
  str referer;
  str retry_after;
  str server;
  str transfer_encoding;
  str user_agent;
  str www_authenticate;
};

//struct Request {
//  std::string method;
//  std::string target;
//  std::string http_version;
//  Header header;
//  std::string body; //?
//};
#endif // HEADER_HPP_