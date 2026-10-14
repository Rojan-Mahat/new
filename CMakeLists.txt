cmake_minimum_required(VERSION 3.20)
project(new LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
add_executable(new
              main.cpp
              Lexer.cpp)