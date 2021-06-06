//
// Created by sfcdo on 05.06.2021.
//

#ifndef WEBSERV_INCLUDES_MESSAGEUTILS_HPP_
#define WEBSERV_INCLUDES_MESSAGEUTILS_HPP_

bool istchar(int c);
bool ishex(int c);
bool ispchar(int c);
bool isquery(int c);
bool isows(int c);
bool isobstext(int c);
bool isvchar(int c);
bool isobsfold(const std::string & s, size_t index);
bool iscrlf(const std::string & s, size_t index);
bool ispctencoded(const std::string & s, size_t index);
bool isqdtext(int c);
bool isquotedpair(const std::string & s, size_t index);
bool isctext(int c);

#endif //WEBSERV_INCLUDES_MESSAGEUTILS_HPP_
