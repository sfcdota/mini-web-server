cmake_minimum_required(VERSION 3.19)
project(parser)

set(CMAKE_CXX_STANDARD 98)

add_executable(parser
        get_next_line.cpp
        parser_new.cpp
        )