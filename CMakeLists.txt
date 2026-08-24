cmake_minimum_required(VERSION 4.2)
project(SystemMonitoringuPrzemysowego)

set(CMAKE_CXX_STANDARD 20)

add_executable(SystemMonitoringuPrzemysowego server.cpp
        client.cpp
        Structures.h)
