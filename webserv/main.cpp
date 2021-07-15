#include "allowed_library_includes.hpp"
#include "Server.hpp"
#include "parser.hpp"
#include "Request.hpp"
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


  return 0;
}