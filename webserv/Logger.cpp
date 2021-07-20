//
// Created by sfcdo on 20.07.2021.
//

#include "Logger.hpp"

Logger::Logger(const output & mode, const std::string &log_path): mode(mode),
  out(mode != STDOUT ? log_path : "tmp", std::ofstream::out | std::ofstream::trunc | std::ofstream::app) {
//  if (mode != STDOUT)
//    out = std::ofstream(mode != STDOUT ? log_path : "tmp", std::ofstream::out | std::ofstream::trunc | std::ofstream::app);
//  else
//    out = std::ofstream();
}
Logger::~Logger() {
  out.close();
}


long Logger::GetTimeInSeconds() {
  gettimeofday(&timev, NULL);
  return timev.tv_sec;
}


//todo timestamp
template<class T, class V>
void Logger::WriteLog(const T &server_unit, const V & loggingLevel) {
  const std::string& log_str = server_unit.WriteLog(loggingLevel);
  switch (mode) {
      case STDOUT: { std::cout << log_str; break; }
      case FILE_ONLY: { out << log_str; break; }
      case STDOUT_AND_FILE: { std::cout << log_str;
                              out << log_str; break; }
    default:
      std::cout << log_str << std::endl; break;
  }
}
