//
// Created by sfcdo on 04.06.2021.
//

#ifndef WEBSERV_MESSAGEPARSER_HPP_
#define WEBSERV_MESSAGEPARSER_HPP_
#include "HeaderUtils.hpp"
#include "Request.hpp"
static std::string request_line_fields [3] = {"method", "target", "version"};


class MessageParser {
 public:
  MessageParser() {};
  ~MessageParser();
  void ProcessHeaders(Request & request);
  void ParseBody(Request & request);
 private:

  std::map<std::string, std::string> & ParseRequestLine();
  std::pair<std::string, std::string> ParseMethod();
  std::pair<std::string, std::string> ParseTarget();
  std::pair<std::string, std::string> ParseVersion();
  void ParseRequestLineField(const std::pair<const std::string, bool (MessageParser::*)()>& pair);
  std::map<std::string, std::string> & ParseHeadersBlock();
  std::pair<std::string, std::string> & ParseHeader();
  void ProcessMethod();
  void ProcessTarget();
  void ProcessVersion();
  void ProcessRequestLine();
  void ParseRequestMessage();
  bool ParseDefaultFieldValue(const std::string & msg, size_t &pos);
  bool ParseAcceptCharsets(const std::string & msg, size_t &pos);
  bool ParseAcceptLanguage(const std::string & msg, size_t &pos);
  bool ParseAllow(const std::string & msg, size_t &pos);
  bool ParseAuthorization(const std::string & msg, size_t &pos);
  bool ParseContentLanguage(const std::string & msg, size_t &pos);
  bool ParseContentLength(const std::string & msg, size_t &pos);
  bool ParseContentLocation(const std::string & msg, size_t &pos);
  bool ParseContentType(const std::string & msg, size_t &pos);
  bool ParseDate(const std::string & msg, size_t &pos);
  bool ParseHost(const std::string & msg, size_t &pos);
  bool ParseLastModified(const std::string & msg, size_t &pos);
  bool ParseLocation(const std::string & msg, size_t &pos);
  bool ParseReferer(const std::string & msg, size_t &pos);
  bool ParseRetryAfter(const std::string & msg, size_t &pos);
  bool ParseServer(const std::string & msg, size_t &pos);
  bool ParseTransferEncoding(const std::string & msg, size_t &pos);
  bool ParseUserAgent(const std::string & msg, size_t &pos);
  bool ParseWwwAuthenticate(const std::string & msg, size_t &pos);
  std::string msg;
  long remain;
  std::map<std::string, bool (MessageParser::*)()> request_line_parse_mapping_;
};
#endif // WEBSERV_MESSAGEPARSER_HPP_
