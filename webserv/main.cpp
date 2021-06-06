#include "allowed_library_includes.hpp"
#include "Server.hpp"

int main(int argc, char **argv) {
  const ServerConfig DEFAULT_CONFIG =
      {0, 8080, std::vector<std::string>(),
       std::vector<error_page>(),
       1024,
       std::vector<location>(),};
  std::vector<ServerConfig> serverConfigs;
  serverConfigs.push_back(DEFAULT_CONFIG);
  Server server(serverConfigs, 2048);
  server.Run();
  return 0;
}