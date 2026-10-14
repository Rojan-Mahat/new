#pragma once
#include<string>
enum categoryType{
    NUMBER,
    IDENTIFIER
};

struct Token{
    std::string original;
    int line;
    categoryType type;
};