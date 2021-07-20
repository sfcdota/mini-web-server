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

struct timeval timev;

enum output{
  STDOUT,
  FILE_ONLY,
  STDOUT_AND_FILE
};

class Logger {
 public:
  Logger(const output & mode = output(), const std::string & log_path = std::string());
  ~Logger();

  template <class T, class V>
  void WriteLog(const T & server_unit, const V & loggingLevel);
  static long GetTimeInSeconds();

 protected:
  const output mode;
  std::ofstream out;
 private:
  Logger(const Logger & in);
  Logger & operator=(const Logger & in);
};

static Logger logger(STDOUT);
#endif //WEBSERV_LOGGER_HPP_
