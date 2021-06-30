#include "allowed_library_includes.hpp"
#include "Server.hpp"

int main(int argc, char **argv) {
  if (argc > 1) // for tests
    exit(0);
  const ServerConfig DEFAULT_CONFIG =
      {0, 8000, std::vector<std::string>(),
       std::vector<error_page>(),
       1024,
       std::vector<location>(),};
  std::vector<ServerConfig> serverConfigs;
  serverConfigs.push_back(DEFAULT_CONFIG);
  Server server(serverConfigs, 2048);
  server.Run();
//  std::string kek = "sec-ch-ua: \" Not;A Brand\";v=\"99\", \"Google Chrome\";v=\"91\", \"Chromium\";v=\"91\"";
//
//  std::string kek2 = "sec-ch-ua-mobile: ?0";
//  size_t pos = 0;
//  std::cout << MessageValidator::ValidHeader(kek2, pos) << std::endl;
  return 0;
}