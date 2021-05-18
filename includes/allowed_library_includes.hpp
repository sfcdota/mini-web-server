#ifndef INCLUDES_ALLOWED_LIBRARY_INCLUDES_HPP_
#define INCLUDES_ALLOWED_LIBRARY_INCLUDES_HPP_


#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
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
#include <exception>
	#include <netdb.h> //????????

int			get_next_line(int fd, std::string str);

#endif // INCLUDES_ALLOWED_LIBRARY_INCLUDES_HPP_
