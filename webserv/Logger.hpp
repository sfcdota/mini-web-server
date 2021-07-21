//
// Created by sfcdo on 20.07.2021.
//

#ifndef WEBSERV_LOGGER_HPP_
#define WEBSERV_LOGGER_HPP_
#include <string>
#include <fstream>
#include <iostream>
#include "Abstract&Interfaces/ILogger.hpp"
#include <sys/time.h>

static struct timeval timev;

enum class output {
  STDOUT,
  FILE_ONLY,
  STDOUT_AND_FILE
};

class Logger {
 public:
  Logger(const output & mode = output(), const std::string & log_path = std::string());
  ~Logger();

//  template <class T, class V>
  template<class V>
  void WriteLog(const ILogger<V> & server_unit, const V & loggingLevel) {
    const std::string& log_str = server_unit.PrintLog(loggingLevel);
    switch (mode) {
      case output::STDOUT: { std::cout << log_str; break; }
      case output::FILE_ONLY: { out << log_str; break; }
      case output::STDOUT_AND_FILE: { std::cout << log_str;
        out << log_str; break; }
      default:
        std::cout << log_str << std::endl; break;
    }
  }
  static long GetTimeInSeconds();

 protected:
  const output mode;
  std::ofstream out;
 private:
  Logger(const Logger & in);
  Logger & operator=(const Logger & in);
};

extern Logger logger;
#endif //WEBSERV_LOGGER_HPP_
