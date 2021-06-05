//
// Created by sfcdo on 05.05.2021.
//

#ifndef CONFIG_HPP_
#define CONFIG_HPP_
#include <string>
#include <vector>
#include "includes/simplifiers.hpp"

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
#endif // CONFIG_HPP_
