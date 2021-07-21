//
// Created by sfcdo on 20.07.2021.
//

#include "Logger.hpp"

Logger::Logger(const output & mode, const std::string &log_path): mode(mode),
  out(mode != output::STDOUT ? log_path : "tmp", std::ofstream::out | std::ofstream::trunc | std::ofstream::app) {
//  if (mode != output::STDOUT)
//    out = std::ofstream(mode != output::STDOUT ? log_path : "tmp", std::ofstream::out | std::ofstream::trunc | std::ofstream::app);
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


////todo timestamp
//template<class V>
//void Logger::WriteLog(const ILogger<V> &server_unit, const V & loggingLevel) {
//  const std::string& log_str = server_unit.PrintLog(loggingLevel);
//  switch (mode) {
//      case output::STDOUT: { std::cout << log_str; break; }
//      case output::FILE_ONLY: { out << log_str; break; }
//      case output::STDOUT_AND_FILE: { std::cout << log_str;
//                              out << log_str; break; }
//    default:
//      std::cout << log_str << std::endl; break;
//  }
//}
Logger logger(output::STDOUT);
