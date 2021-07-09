//
// Created by sfcdo on 04.06.2021.
//

#ifndef WEBSERV_MESSAGEPARSER_HPP_
#define WEBSERV_MESSAGEPARSER_HPP_
#include "allowed_library_includes.hpp"
#include "MessageUtils.hpp"
#include "Request.hpp"


class MessageParser {
 public:
  MessageParser();
  ~MessageParser();
  Request ProcessRequest(std::string & request);
 private:

  std::map<std::string, std::string> & ParseRequestLine();
  std::pair<std::string, std::string> ParseMethod();
  std::pair<std::string, std::string> ParseTarget();
  std::pair<std::string, std::string> ParseVersion();
  void ParseRequestLineField(const std::pair<const std::string, bool (MessageParser::*)()>& pair);
  std::map<std::string, std::string> & ParseHeadersBlock();
  std::pair<std::string, std::string> & ParseHeader();
  std::string & ParseBody();
  void ProcessMethod();
  void ProcessTarget();
  void ProcessVersion();
  void ProcessRequestLine();
  void ParseRequestMessage();
  std::string msg;
  Request request_;
  std::map<std::string, bool (MessageParser::*)()> request_line_parse_mapping_;
};
#endif // WEBSERV_MESSAGEPARSER_HPP_
