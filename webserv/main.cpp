#include "allowed_library_includes.hpp"
#include "Server.hpp"
#include "parser.hpp"
int main(int argc, char **argv) {
  if (argc > 1) // for tests
    exit(0);
  ServerConfig config = parsConf();
  const ServerConfig DEFAULT_CONFIG =
      {0, 8080, std::string(),
       std::vector<std::string>(),
       std::vector<error>(),
       8096,
       std::vector<std::string>(),
       std::vector<location>(),};
  std::vector<ServerConfig> serverConfigs;
//  serverConfigs.push_back(DEFAULT_CONFIG);
serverConfigs.push_back(config);
  Server server(serverConfigs, 2048);
  server.Run();
//  std::string kek = "sec-ch-ua: \" Not;A Brand\";v=\"99\", \"Google Chrome\";v=\"91\", \"Chromium\";v=\"91\"";
//
//  std::string kek2 = "sec-ch-ua-mobile: ?0";
//  size_t pos = 0;
//  std::cout << MessageValidator::ValidHeader(kek2, pos) << std::endl;

  return 0;
}