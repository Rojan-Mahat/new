#pragma once
#include "Token.hpp"
#include<vector>

class Lexer{
    private:
    std::string srcCode;
    std::size_t cursor;
    int lineNo;
    std::vector<Token> tokens;



    void scanLetter();
    void scanNumber();


    public:
    Lexer(const std::string& src);
    std::vector<Token> tokenize();
    
};




