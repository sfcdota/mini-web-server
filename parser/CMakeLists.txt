cmake_minimum_required(VERSION 3.19)
project(parser)

set(CMAKE_CXX_STANDARD 98)

add_executable(parser
#        main.cpp
        parser.cpp
        get_next_line.cpp
        )