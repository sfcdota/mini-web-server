//
// Created by sfcdo on 04.06.2021.
//

#ifndef HTTP_MESSAGEANALYZER_HPP_
#define HTTP_MESSAGEANALYZER_HPP_
#include "includes/allowed_library_includes.hpp"

const char *tchar = "!#$%&'*+-.^_`|~";
const char *pchar = "!$&'()*+,;=-._~:@%";

class HTTP_MessageAnalyzer {
 public:
  HTTP_MessageAnalyzer(const std::string & message, const bool isRequest);
  ~HTTP_MessageAnalyzer();
 private:
  HTTP_MessageAnalyzer();
  bool IsValidRequest(std::string & request);
  bool ValidRequestLine();
  bool ValidMethod();
  bool ValidTarget();
  bool ValidVersion();
  bool ValidHeaders();
  bool ValidBody();
  bool istchar(int c);
  bool ishex(int c);
  bool ispchar(int c);
  bool isquery(int c);
  bool isows(int c);
  bool ispctencoded();
  const std::string msg;
  const bool isRequest;
  size_t begin;
  const size_t end;
  size_t at_least_one;
};
#endif // HTTP_MESSAGEANALYZER_HPP_
