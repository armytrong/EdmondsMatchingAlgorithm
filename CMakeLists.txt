cmake_minimum_required(VERSION 3.27)
project(EdmondsMatchingAlgorithm)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(EdmondsMatchingAlgorithm
        graph.cpp
        graph.h
        main.cpp
        dimacs_file_parser.cpp
        dimacs_file_parser.h
)
