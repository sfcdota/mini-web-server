//
// Created by sfcdo on 08.06.2021.
//

#ifndef WEBSERV_MESSAGEVALIDATOR_HPP_
#define WEBSERV_MESSAGEVALIDATOR_HPP_
#include <string>

class MessageValidator {
 public:
  MessageValidator();
  ~MessageValidator();
  bool ValidHeaders(const std::string & request);
  bool ValidResponse(const std::string & response);
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
  static bool ValidBody(const std::string &msg);
  static bool ValidDefaultFieldValue(const std::string & msg, size_t &pos);
  static bool CheckAcceptCharsets(const std::string & msg, size_t &pos);
  static bool CheckAcceptLanguage(const std::string & msg, size_t &pos);
  static bool CheckAllow(const std::string & msg, size_t &pos);
  static bool CheckAuthorization(const std::string & msg, size_t &pos);
  static bool CheckContentLanguage(const std::string & msg, size_t &pos);
  static bool CheckContentLength(const std::string & msg, size_t &pos);
  static bool CheckContentLocation(const std::string & msg, size_t &pos);
  static bool CheckContentType(const std::string & msg, size_t &pos);
  static bool CheckDate(const std::string & msg, size_t &pos);
  static bool CheckHost(const std::string & msg, size_t &pos);
  static bool CheckLastModified(const std::string & msg, size_t &pos);
  static bool CheckLocation(const std::string & msg, size_t &pos);
  static bool CheckReferer(const std::string & msg, size_t &pos);
  static bool CheckRetryAfter(const std::string & msg, size_t &pos);
  static bool CheckServer(const std::string & msg, size_t &pos);
  static bool CheckTransferEncoding(const std::string & msg, size_t &pos);
  static bool CheckUserAgent(const std::string & msg, size_t &pos);
  static bool CheckWwwAuthenticate(const std::string & msg, size_t &pos);

  static bool isdots(int c, int = 0);
  static bool istchar(int c, int = 0);
  static bool ishex(int c, int = 0);
  static bool ispchar(int c, int = 0);
  static bool isquery(int c, int = 0);
  static bool isows(int c, int = 0);
  static bool isobstext(int c, int = 0);
  static bool isvchar(int c, int = 0);
  static bool isobsfold(const std::string & s, size_t & index);
  static bool iscrlf(const std::string & s, size_t index);
  static bool ispctencoded(const std::string & s, size_t & index);
  static bool isqdtext(int c, int = 0);
  static bool isctext(int c, int = 0);
  static bool isreasonphrase(int c, int = 0);
  static bool isquotedpair(const std::string &s, size_t & index);
  static bool isstatuscode(const std::string &s, size_t & index);

 private:
  std::string message;
  size_t msg_pos;
  static size_t status_code;
  static size_t out_temp;


  template<class ret, class kek>
  bool exp_test_call(const std::string & s, size_t & index, ret (kek::*f)(int, int));
  template<class ret, class kek, class T1, class T2>
  bool exp_test_call(const std::string & s, size_t & index, ret (kek::*f)(T1, T2));

  template<class ret>
  static bool exp_test_call(const std::string & s, size_t & index, ret (*f)(int, int));
  template<class ret, class T1, class T2>
  static bool exp_test_call(const std::string & s, size_t & index, ret (*f)(T1, T2));

  template<class ret, class kek, class T1, class T2>
  bool expression_test(const std::string & s, size_t & index, ret (kek::*f)(T1, T2),
      size_t status_code = 0, size_t step = 1, size_t min = 0, size_t max = -2);

  template<class ret, class T1, class T2>
  static bool expression_test(const std::string & s, size_t & index, ret (*f)(T1, T2), size_t & out_status_code_ref,
                       size_t status_code = 0, size_t step = 1, size_t min = 0, size_t max = -2);

};
#endif //WEBSERV_MESSAGEVALIDATOR_HPP_
