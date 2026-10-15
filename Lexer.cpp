
#include"Lexer.hpp"
#include"Token.hpp"
#include<cctype>
#include <stdexcept>
#include<unordered_map>
#include<unordered_set>
#include <string_view>

namespace{
    constexpr std::string_view operatorStarts = "+-*/<>=:";

    const std::unordered_map<std::string,categoryType> keywords= {
        {"make",categoryType::MAKE},
        {"when", categoryType::WHEN},
        {"otherwise", categoryType::OTHERWISE},
        {"repeat_until", categoryType::REPEAT_UNTIL},
        {"task", categoryType::TASK},
    };

     const std::unordered_map<std::string, categoryType> operators = {
        {"+", categoryType::PLUS},
        {"-", categoryType::MINUS},
        {"*", categoryType::MULTIPLY},
        {"/", categoryType::DIVIDE},

        {"<", categoryType::SMALLER_THAN},
        {">", categoryType::GREATER_THAN},
        {"<=", categoryType::SMALLER_OR_EQUAL_TO},
        {">=", categoryType::GREATER_OR_EQUAL_TO},

        {":=", categoryType::ASSIGNMENT},
        {"++", categoryType::INCREMENT},
        {"--", categoryType::DECREMENT},
        {"==", categoryType::EQUAL_EQUAL},
        
        {"+=", categoryType::ADDITION_ASSIGNMENT},
        {"-=", categoryType::SUBTRACTION_ASSIGNMENT},
        {"*=", categoryType::MULTIPLICATION_ASSIGNMENT},
        {"/=", categoryType::DIVISION_ASSIGNMENT},
    };

    const std::unordered_map<char , categoryType> punctuation = {
        {'(',categoryType::LEFT_PAREN},
        { ')',categoryType::RIGHT_PAREN}, 
        {'{',categoryType::LEFT_BRACE}, 
        {'}',categoryType::RIGHT_BRACE}, 
        {'[',categoryType::LEFT_SQUARE}, 
        {']',categoryType::RIGHT_SQUARE}, 
        { ',',categoryType::COMMA}, 
        {';',categoryType::SEMICOLON},
    };
}


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
        else if(operatorStarts.find(srcCode[cursor]) != std::string_view::npos)
            scanOperator();
        
        else if(punctuation.contains(srcCode[cursor]))
            scanPunctuation();

        else if(srcCode[cursor]=='"')
            scanString();

        else if(srcCode[cursor]=='~')
            handleComments();

        else if(srcCode[cursor] == '\n'){
            lineNo++;
            cursor++;
        }

        else if (std::isspace(static_cast<unsigned char>(srcCode[cursor]))){
            cursor++;
        }

        else
            throw std::runtime_error("Lexer Error: unknown operator found at "+ std::to_string(lineNo));
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
    if(cursor < srcCode.size() && (std::isalpha(static_cast<unsigned char>(srcCode[cursor])) || srcCode[cursor]=='_')){
        throw std::runtime_error("Lexer Error: seperate letters with numbers at line "+ std::to_string(lineNo));
    }
    std::string org = srcCode.substr(numberStart, cursor-numberStart);
    tokens.push_back(Token{org, lineNo, categoryType::NUMBER});
}

void Lexer::scanOperator(){
    std::string twoChar = srcCode.substr(cursor, 2);
    auto twoCharOperator =operators.find(twoChar);
    if(twoCharOperator !=operators.end()){
        tokens.push_back(Token{twoChar, lineNo, twoCharOperator->second});
        cursor+=2;
    }
    else{
        std::string oneChar = srcCode.substr(cursor, 1);
        auto oneCharOperator = operators.find(oneChar);
        if(oneCharOperator != operators.end()){
            tokens.push_back(Token{oneChar, lineNo, oneCharOperator ->second});
            cursor++;
        }
        else
            throw std::runtime_error("Lexer Error: unknown operator found at "+ std::to_string(lineNo));
    }
}

void Lexer::scanLetter() {
    std::size_t letterStart = cursor;
    while(cursor<srcCode.size() && (std::isalpha(static_cast<unsigned char>(srcCode[cursor])) || srcCode[cursor]=='_' || std::isdigit(static_cast<unsigned char>(srcCode[cursor]))))
        cursor++;
    std::string org = srcCode.substr(letterStart, cursor-letterStart);
    auto it = keywords.find(org);
    if(it != keywords.end())
        tokens.push_back(Token{org, lineNo, it->second});
    else
        tokens.push_back(Token{org, lineNo, categoryType::IDENTIFIER});
}

void Lexer::scanPunctuation(){
    std::string org = std::string(1, srcCode[cursor]);
    tokens.push_back(Token{org, lineNo, punctuation.at(srcCode[cursor])});
    cursor++;
}

void Lexer::handleComments(){
    while(cursor < srcCode.size() && srcCode[cursor]!='\n'){
        cursor++;
    }
}


 void Lexer::scanString(){
    cursor++;
    std::size_t start = cursor;
    int startingLine = lineNo;
    while(cursor < srcCode.size() && srcCode[cursor]!='"'){
        if(srcCode[cursor]=='\\'){
            if(cursor+1 < srcCode.size()){
                if(srcCode[cursor+1]=='"' || srcCode[cursor+1]=='n' || srcCode[cursor+1]=='t' || srcCode[cursor+1]=='\\')
                    cursor++;
                else{
                    std::string org = srcCode.substr(cursor,2);
                    throw std::runtime_error("Lexer error: unknown sequence "+org+ " found at "+ std::to_string(lineNo));
                }
            }
            else
                throw std::runtime_error("Lexer error:  Incomplete escape sequence at line "+ std::to_string(lineNo));
        }
        else if(srcCode[cursor] == '\n')
            lineNo++;
        cursor++;
    }
    if(cursor < srcCode.size()){
        std::string org = srcCode.substr(start,cursor-start);
        tokens.push_back(Token{org, startingLine, categoryType::STRING});
        cursor++;
    }
    else
        throw std::runtime_error("Lexer error: string opened at "+ std::to_string(startingLine));
 }

