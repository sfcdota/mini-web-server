//
// Created by sfcdo on 02.07.2021.
//

#ifndef WEBSERV_HEADERVALIDATOR_HPP_
#define WEBSERV_HEADERVALIDATOR_HPP_
#include <map>
#include <string>

class HeaderValidator {
 public:
  explicit HeaderValidator(std::map<std::string, std::string> & header_map);
  bool CheckAcceptCharsets(std::map<std::string, std::string> & header_map);
  bool CheckAcceptLanguage(std::map<std::string, std::string> &header_map);
  bool CheckAllow(std::map<std::string, std::string> &header_map);
  bool CheckAuthorization(std::map<std::string, std::string> &header_map);
  bool CheckContentLanguage(std::map<std::string, std::string> &header_map);
  bool CheckContentLength(std::map<std::string, std::string> &header_map);
  bool CheckContentLocation(std::map<std::string, std::string> &header_map);
  bool CheckContentType(std::map<std::string, std::string> &header_map);
  bool CheckDate(std::map<std::string, std::string> &header_map);
  bool CheckHost(std::map<std::string, std::string> &header_map);
  bool CheckLastModified(std::map<std::string, std::string> &header_map);
  bool CheckLocation(std::map<std::string, std::string> &header_map);
  bool CheckReferer(std::map<std::string, std::string> &header_map);
  bool CheckRetryAfter(std::map<std::string, std::string> &header_map);
  bool CheckServer(std::map<std::string, std::string> &header_map);
  bool CheckTransferEncoding(std::map<std::string, std::string> &header_map);
  bool CheckUserAgent(std::map<std::string, std::string> &header_map);
  bool CheckWwwAuthenticate(std::map<std::string, std::string> &header_map);
 private:
  std::map<std::string, std::string> & header_map;
};
#endif //WEBSERV_HEADERVALIDATOR_HPP_
