#pragma once

#include<stdexcept>
#include<string>

class ParserError : public std::runtime_error{
    private:
    int lineNo;

    public:
    ParserError(const std::string& msg , int line)
        :std::runtime_error(msg),
        lineNo(line)
    {}

    int getLine() const{
        return lineNo;
    }

};