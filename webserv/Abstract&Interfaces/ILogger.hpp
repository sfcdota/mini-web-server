//
// Created by sfcdo on 20.07.2021.
//

#ifndef WEBSERV_ABSTRACT_INTERFACES_ILOGGER_HPP_
#define WEBSERV_ABSTRACT_INTERFACES_ILOGGER_HPP_
#include <string>

std::string GetShortString(const std::string & s) {
  return s.substr(0, 100) + (s.size() > 100 ? "..." : "");
}


template<class T>
class ILogger {
 public:
  virtual const std::string PrintLog(const int & logginglevel, const T & option) const = 0;
};
#endif //WEBSERV_ABSTRACT_INTERFACES_ILOGGER_HPP_
