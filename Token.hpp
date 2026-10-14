#pragma once
#include<string>
enum categoryType{
    NUMBER,
    IDENTIFIER,
    PLUS,
    INCREMENT,
    DECREMENT,
    MINUS,
    MULTIPLY,
    DIVIDE,
    ASSIGNMENT,
    EQUAL_EQUAL,
    GREATER_THAN,
    GREATER_OR_EQUAL_TO,
    SMALLER_THAN,
    SMALLER_OR_EQUAL_TO,
    ADDITION_ASSIGNMENT,
    SUBTRACTION_ASSIGNMENT,
    MULTIPLICATION_ASSIGNMENT,
    DIVISION_ASSIGNMENT,
    
};

struct Token{
    std::string original;
    int line;
    categoryType type;
};

std::string tokenTypeToString(categoryType type);