# cmake_minimum_required(VERSION <specify CMake version here>)
project(webserv)
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "-D BUFFER_SIZE=256 -Wall -Wextra -Werror -stdlib=libc++")
#set(CMAKE_CXX_FLAGS "-stdlib=libc++ -v")
set(CMAKE_CXX_FLAGS "-stdlib=libc++")
set(CMAKE_CXX_COMPILER "clang++")

#Utils sources
#file(GLOB libft "libft/*.c")
#file(GLOB gnl "get_next_line/*.c")

#set_source_files_properties(${libft} ${gnl} PROPERTIES LANGUAGE CXX )

file(GLOB main "webserv/*.cpp")
file(GLOB conf "webserv/parser/*.cpp")
file(GLOB abstract "webserv/Abstract&Interfaces/*.cpp")

include_directories(webserv/includes
                    /usr/local/include
                    /usr/include
                    webserv/
                    webserv/Abstract&Interfaces)

#Utils headers
#include_directories(libft/)
#include_directories(get_next_lin

add_executable(webserv
#        ${gnl} ${libft}
        ${conf} ${abstract} ${main}
        )
