cmake_minimum_required(VERSION 3.20)
project(new LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(new
              main.cpp
              Lexer.cpp
              Token.cpp
              Parser.cpp
              Evaluator.cpp
              Environment.cpp)
              
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

add_executable(environment_tests
    tests/environment_test.cpp
    Environment.cpp
)

target_include_directories(environment_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(interpreter_tests
              tests/interpreter_tests.cpp
              Lexer.cpp
              Token.cpp
              Parser.cpp
              Evaluator.cpp
              Environment.cpp)

target_include_directories(interpreter_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
enable_testing()
add_test(NAME lexer COMMAND lexer_tests)
add_test(NAME parser COMMAND parser_tests)
add_test(NAME environment COMMAND environment_tests)
add_test(NAME interpreter COMMAND interpreter_tests)
