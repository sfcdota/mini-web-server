//
// Created by sfcdo on 08.06.2021.
//

#ifndef WEBSERV_MESSAGEVALIDATOR_HPP_
#define WEBSERV_MESSAGEVALIDATOR_HPP_
#include <string>
#include "MessageUtils.hpp"

class MessageValidator {
 public:
  MessageValidator();
  ~MessageValidator();
  bool ValidRequest(std::string & request);
  bool ValidResponse(std::string & response);
 private:
  std::string message;
  size_t msg_pos;
  size_t status_code;
 public:
  size_t GetStatusCode() const;
// private:
  static bool ValidRequestLine(const std::string &msg, size_t &pos);
  static bool ValidStatusLine(const std::string &msg, size_t &pos);
  static bool ValidMethod(const std::string &msg, size_t &pos);
  static bool ValidTarget(const std::string &msg, size_t &pos);
  static bool ValidVersion(const std::string &msg, size_t &pos);
  static bool ValidHeadersBlock(const std::string &msg, size_t &pos);
  static bool ValidHeader(const std::string &msg, size_t &pos);
  static bool ValidFieldName(const std::string &msg, size_t &pos);
  static bool ValidFieldValue(const std::string &msg, size_t &pos);
  static bool ValidQuotedString(const std::string &msg, size_t &pos);
  static bool ValidComment(const std::string &msg, size_t &pos);
  static bool ValidFieldContent(const std::string &msg, size_t &pos);
  static bool ValidBody(const std::string &msg, size_t &pos);
};
#endif //WEBSERV_MESSAGEVALIDATOR_HPP_
