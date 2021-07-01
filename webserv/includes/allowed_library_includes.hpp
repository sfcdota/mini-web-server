#ifndef WEBSERV_INCLUDES_ALLOWED_LIBRARY_INCLUDES_HPP_
#define WEBSERV_INCLUDES_ALLOWED_LIBRARY_INCLUDES_HPP_
#include <cstdlib>
#include <sys/stat.h>
//#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <dirent.h>
#include <ctime>
#include <cstring>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <algorithm>
#include <fstream>
#include <exception>
#include "../../parser/parser.hpp"
	#include <netdb.h> //????????

//  #include "libft.h"
//  #include "get_next_line.h"
#define MAX_CONNECTIONS 128
#define TIMOUT_USEC 100
#define DEFAULT_INPUT_BUFFERSIZE 2048
#endif // WEBSERV_INCLUDES_ALLOWED_LIBRARY_INCLUDES_HPP_
