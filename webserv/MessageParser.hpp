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
  Request ProcessRequest(std::string & http_message);
 private:
  bool ValidRequestLine();
  bool ValidMethod();
  bool ValidTarget();
  bool ValidVersion();
  bool ValidHeadersBlock();
  bool ValidHeader();
  bool ValidFieldName();
  bool ValidFieldValue();
  bool ValidQuotedString();
  bool ValidComment();
  bool ValidFieldContent();
  bool ValidBody();
  std::map<std::string, std::string> & ParseRequestLine();
  std::pair<string, string> ParseMethod();
  std::pair<string, string> ParseTarget();
  std::pair<string, string> ParseVersion();
  void ParseRequestLineField(const std::pair<const std::string, bool (MessageParser::*)()>& pair);
  std::map<std::string, std::string> & ParseHeadersBlock();
  std::pair<std::string, std::string> & ParseHeader();
  std::string & ParseBody();
  void ProcessMethod();
  void ProcessTarget();
  void ProcessVersion();
  void ProcessRequestLine();
  std::string msg;
  Request request_;
  size_t begin;
  size_t pos;
  size_t end;
  std::map<std::string, bool (MessageParser::*)()> request_line_parse_mapping_;
};
#endif // WEBSERV_MESSAGEPARSER_HPP_
