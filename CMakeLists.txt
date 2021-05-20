# cmake_minimum_required(VERSION <specify CMake version here>)
project(webserv)
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 98)
#set(CMAKE_CXX_FLAGS "-D BUFFER_SIZE=256 -Wall -Wextra -Werror -stdlib=libc++")
set(CMAKE_CXX_FLAGS "-D BUFFER_SIZE=256 -stdlib=libc++")
set(CMAKE_CXX_COMPILER "clang++")

#Utils sources
file(GLOB libft "libft/*.c")
file(GLOB gnl "get_next_line/*.c")

set_source_files_properties(${libft} ${gnl} PROPERTIES LANGUAGE CXX )

file(GLOB include "includes/*")
file(GLOB main "*.cpp")
include_directories(includes /usr/local/include)

#Utils headers
include_directories(libft/)
include_directories(get_next_line/)

add_executable(webserv
        ${include} ${gnl} ${libft} ${main})
