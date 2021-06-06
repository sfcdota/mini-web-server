//
// Created by sfcdo on 05.05.2021.
//

#ifndef WEBSERV_CONFIG_HPP_
#define WEBSERV_CONFIG_HPP_
#include <string>
#include <vector>
#include "simplifiers.hpp"

//Full public class ?
class Config {
public:
    int host;
    int port;
    std::vector<std::string> location;
    int client_body_limit;
    std::vector<std::string> error_page;

private:
};
#endif // WEBSERV_CONFIG_HPP_
