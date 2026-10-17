#pragma once
#include <stdexcept>
#include<string>
class LexerError : public std::runtime_error{
    private:
    int lineNo;

    public:
    LexerError(const std::string& message, int line)
        :std::runtime_error(message),
        lineNo(line)
        {}
        
    int getLine() const{
        return lineNo;
    }
};
