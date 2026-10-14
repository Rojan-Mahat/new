
#include"Lexer.hpp"
#include"Token.hpp"
#include<cctype>
#include <stdexcept>

Lexer::Lexer(const std::string& src)
        :srcCode(src),
        cursor(0),
        lineNo(1)
    {
    }
std::vector<Token> Lexer::tokenize(){
    while(cursor < srcCode.size()){
        if(std::isdigit(static_cast<unsigned char>(srcCode[cursor])) || srcCode[cursor] == '.') {
            scanNumber();
        }
        else if(std::isalpha(static_cast<unsigned char>(srcCode[cursor])) || srcCode[cursor]=='_'){
            scanLetter();
        }
        else if(srcCode[cursor] == '\n'){
            lineNo++;
            cursor++;
        }
        else{
            cursor++;
        }
    }
    return std::move(tokens);
}


void Lexer::scanNumber(){
    std::size_t numberStart = cursor;
    bool dot = false;
    while(cursor < srcCode.size() && (std::isdigit(static_cast<unsigned char>(srcCode[cursor])) || srcCode[cursor]=='.')){
        if(srcCode[cursor]=='.'){
            if(dot || cursor+1 >= srcCode.size() || cursor == numberStart)
                throw std::runtime_error("Lexer Error: Invalid or misplaced decimal point at line "+std::to_string(lineNo));
            if(!std::isdigit(static_cast<unsigned char>(srcCode[cursor+1])) || !std::isdigit(static_cast<unsigned char>(srcCode[cursor-1])))
                throw std::runtime_error("Lexer Error: numbers must be before and after '.' at line " + std::to_string(lineNo));
            dot = true;
        }
        cursor++;
    }
    if(cursor < srcCode.size() && std::isalpha(static_cast<unsigned char>(srcCode[cursor]))){
        throw std::runtime_error("Lexer Error: seperate letters with numbers at line "+ std::to_string(lineNo));
    }
    std::string org = srcCode.substr(numberStart, cursor-numberStart);
    tokens.push_back(Token{org, lineNo, categoryType::NUMBER});
}

void Lexer::scanLetter() {
    std::size_t letterStart = cursor;
    while(cursor<srcCode.size() && (std::isalpha(static_cast<unsigned char>(srcCode[cursor])) || srcCode[cursor]=='_' || std::isdigit(static_cast<unsigned char>(srcCode[cursor]))))
        cursor++;
    std::string org = srcCode.substr(letterStart, cursor-letterStart);
    tokens.push_back(Token{org, lineNo, categoryType::IDENTIFIER});
}



