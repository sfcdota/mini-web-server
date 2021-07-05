//
// Created by sfcdo on 05.06.2021.
//

#ifndef WEBSERV_INCLUDES_HEADERUTILS_HPP_
#define WEBSERV_INCLUDES_HEADERUTILS_HPP_

bool istchar(int c);
bool ishex(int c);
bool ispchar(int c);
bool isquery(int c);
bool isows(int c);
bool isobstext(int c);
bool isvchar(int c);
bool isobsfold(const std::string & s, size_t & index);
bool iscrlf(const std::string & s, size_t index);
bool ispctencoded(const std::string & s, size_t & index);
bool isqdtext(int c);
bool isquotedpair(const std::string & s, size_t & index);
bool isctext(int c);
bool isreasonphrase(int c);
bool isstatuscode(const std::string &s, size_t & index);

bool expression_test(const std::string & s, size_t & index, bool (* func)(int), size_t status_code = 0, size_t step = 1, size_t min = 0, size_t max = -2);
bool expression_test(const std::string & s, size_t & index, bool (* func)(const std::string &, size_t &),
                     size_t status_code = 0, size_t step = 1, size_t min = 0, size_t max = -2);
bool expression_test(const std::string & s, size_t & index, bool t,
                     size_t status_code = 0, size_t step = 1, size_t min = 0, size_t max = -2);
#endif //WEBSERV_INCLUDES_HEADERUTILS_HPP_
