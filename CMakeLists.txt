cmake_minimum_required(VERSION 3.22)
project(CEdit)

set(CMAKE_CXX_STANDARD 20)

add_executable(CEdit src/main.cpp
        src/environment.hpp
        src/file.hpp
        src/core.hpp)