# cmake_minimum_required(VERSION <specify CMake version here>)
project(webserv)
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 98)
#set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -stdlib=libc++")
set(CMAKE_CXX_FLAGS "-stdlib=libc++")
set(CMAKE_CXX_COMPILER "clang++")

file(GLOB include "includes/*")


include_directories(includes /usr/local/include)

add_executable(webserv
        ${include}
        main.cpp ConfigReader.cpp ConfigReader.hpp Config.cpp Header.cpp)
