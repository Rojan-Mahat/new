cmake_minimum_required(VERSION 3.20)
project(new LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(new
              main.cpp
              Lexer.cpp
              Token.cpp
              Parser.cpp)
add_executable(lexer_tests
    tests/lexer_tests.cpp
    Lexer.cpp
    Token.cpp
)

target_include_directories(lexer_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(parser_tests
    tests/parser_tests.cpp
    Lexer.cpp
    Parser.cpp
    Token.cpp
)

target_include_directories(parser_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)