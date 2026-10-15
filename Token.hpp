#pragma once
#include<string>
enum categoryType{
    NUMBER,
    IDENTIFIER,

    ASSIGNMENT,
    PLUS,
    INCREMENT,
    DECREMENT,
    MINUS,
    MULTIPLY,
    DIVIDE,

    ADDITION_ASSIGNMENT,
    SUBTRACTION_ASSIGNMENT,
    MULTIPLICATION_ASSIGNMENT,
    DIVISION_ASSIGNMENT,

    EQUAL_EQUAL,
    GREATER_THAN,
    GREATER_OR_EQUAL_TO,
    SMALLER_THAN,
    SMALLER_OR_EQUAL_TO,
    


    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_SQUARE,
    RIGHT_SQUARE,
    COMMA,
    SEMICOLON,


    MAKE,
    WHEN,
    OTHERWISE,
    REPEAT_UNTIL,
    TASK,


    STRING,
};



struct Token{
    std::string original;
    int line;
    categoryType type;
};

std::string tokenTypeToString(categoryType type);