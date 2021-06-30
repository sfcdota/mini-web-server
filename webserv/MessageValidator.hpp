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
  bool ValidRequestLine(const std::string &msg, size_t &pos);
  bool ValidStatusLine(const std::string &msg, size_t &pos);
  bool ValidMethod(const std::string &msg, size_t &pos);
  bool ValidTarget(const std::string &msg, size_t &pos);
  bool ValidVersion(const std::string &msg, size_t &pos);
  bool ValidHeadersBlock(const std::string &msg, size_t &pos);
  bool ValidHeader(const std::string &msg, size_t &pos);
  bool ValidFieldName(const std::string &msg, size_t &pos);
  bool ValidFieldValue(const std::string &msg, size_t &pos);
  bool ValidQuotedString(const std::string &msg, size_t &pos);
  bool ValidComment(const std::string &msg, size_t &pos);
  bool ValidFieldContent(const std::string &msg, size_t &pos);
  bool ValidBody(const std::string &msg, size_t &pos);
};
#endif //WEBSERV_MESSAGEVALIDATOR_HPP_
