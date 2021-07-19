//
// Created by sfcdo on 08.06.2021.
//

#ifndef WEBSERV_MESSAGEVALIDATOR_HPP_
#define WEBSERV_MESSAGEVALIDATOR_HPP_
#include <string>
#include "sfinae.hpp"
//#include "HeaderUtils.hpp"

class MessageValidator {
 public:
  MessageValidator();
  ~MessageValidator();
  bool ValidHeaders(std::string & request);
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
  bool ValidBody(const std::string &msg);
  bool ValidDefaultFieldValue(const std::string & msg, size_t &pos);
  bool CheckAcceptCharsets(const std::string & msg, size_t &pos);
  bool CheckAcceptLanguage(const std::string & msg, size_t &pos);
  bool CheckAllow(const std::string & msg, size_t &pos);
  bool CheckAuthorization(const std::string & msg, size_t &pos);
  bool CheckContentLanguage(const std::string & msg, size_t &pos);
  bool CheckContentLength(const std::string & msg, size_t &pos);
  bool CheckContentLocation(const std::string & msg, size_t &pos);
  bool CheckContentType(const std::string & msg, size_t &pos);
  bool CheckDate(const std::string & msg, size_t &pos);
  bool CheckHost(const std::string & msg, size_t &pos);
  bool CheckLastModified(const std::string & msg, size_t &pos);
  bool CheckLocation(const std::string & msg, size_t &pos);
  bool CheckReferer(const std::string & msg, size_t &pos);
  bool CheckRetryAfter(const std::string & msg, size_t &pos);
  bool CheckServer(const std::string & msg, size_t &pos);
  bool CheckTransferEncoding(const std::string & msg, size_t &pos);
  bool CheckUserAgent(const std::string & msg, size_t &pos);
  bool CheckWwwAuthenticate(const std::string & msg, size_t &pos);

  bool isdots(int c, int = 0);
  bool istchar(int c, int = 0);
  bool ishex(int c, int = 0);
  bool ispchar(int c, int = 0);
  bool isquery(int c, int = 0);
  bool isows(int c, int = 0);
  bool isobstext(int c, int = 0);
  bool isvchar(int c, int = 0);
  bool isobsfold(const std::string & s, size_t & index);
  bool iscrlf(const std::string & s, size_t index);
  bool ispctencoded(const std::string & s, size_t & index);
  bool isqdtext(int c, int = 0);
  bool isctext(int c, int = 0);
  bool isreasonphrase(int c, int = 0);
  bool isquotedpair(const std::string &s, size_t & index);
  bool isstatuscode(const std::string &s, size_t & index);

 private:
  size_t temp;



  template<class ret, class kek>
  bool exp_test_call(const std::string & s, size_t & index, ret (kek::*f)(int, int));
  template<class ret, class kek, class T1, class T2>
  bool exp_test_call(const std::string & s, size_t & index, ret (kek::*f)(T1, T2));



  template<class ret, class kek, class T1, class T2>
  bool expression_test(const std::string & s, size_t & index, ret (kek::*f)(T1, T2),
      size_t status_code = 0, size_t step = 1, size_t min = 0, size_t max = -2);


};
#endif //WEBSERV_MESSAGEVALIDATOR_HPP_
