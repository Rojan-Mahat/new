
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
        else if(srcCode[cursor]=='<' || srcCode[cursor]=='>' ||srcCode[cursor]== ':'|| 
            srcCode[cursor]=='+' || srcCode[cursor]=='-'|| srcCode[cursor]=='*'|| 
            srcCode[cursor]=='/'|| srcCode[cursor]=='=')
            scanOperator();

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

void Lexer::scanOperator(){
    if(srcCode[cursor] == ':'){
        if(cursor+1 >= srcCode.size())
            throw std::runtime_error("Lexer Error: operator at line "+ std::to_string(lineNo)+" not defined");
        if(srcCode[cursor+1] == '='){
            tokens.push_back(Token{":=",lineNo,categoryType::ASSIGNMENT});
            cursor+=2;
        }
        else
            throw std::runtime_error("Lexer Error: operator at line "+ std::to_string(lineNo)+" not defined");  
    }

    else if(srcCode[cursor] == '<'){
        if(cursor+1 < srcCode.size() && srcCode[cursor+1] == '='){
            tokens.push_back(Token{"<=",lineNo,categoryType::SMALLER_OR_EQUAL_TO});
            cursor+=2;
            return;
        }
        tokens.push_back(Token{"<",lineNo,categoryType::SMALLER_THAN});
        cursor++;
    }
    else if(srcCode[cursor] == '>'){
        if(cursor+1 < srcCode.size() && srcCode[cursor+1] == '='){
            tokens.push_back(Token{">=",lineNo,categoryType::GREATER_OR_EQUAL_TO});
            cursor+=2;
            return;
        }
        tokens.push_back(Token{">",lineNo,categoryType::GREATER_THAN});
        cursor++;
    }
    else if(srcCode[cursor] == '+'){
       if(cursor+1 < srcCode.size() && srcCode[cursor+1] == '+'){
            tokens.push_back(Token{"++",lineNo,categoryType::INCREMENT});
            cursor+=2;
            return;
        }
        if(cursor+1 < srcCode.size() && srcCode[cursor+1] == '='){
            tokens.push_back(Token{"+=",lineNo,categoryType::ADDITION_ASSIGNMENT});
            cursor+=2;
            return;
        }
        tokens.push_back(Token{"+",lineNo,categoryType::PLUS});
        cursor++;
    }
    else if(srcCode[cursor] == '-'){
       if(cursor+1 < srcCode.size() && srcCode[cursor+1] == '-'){
            tokens.push_back(Token{"--",lineNo,categoryType::DECREMENT});
            cursor+=2;
            return;
        }
        if(cursor+1 < srcCode.size() && srcCode[cursor+1] == '='){
            tokens.push_back(Token{"-=",lineNo,categoryType::SUBTRACTION_ASSIGNMENT});
            cursor+=2;
            return;
        }
        tokens.push_back(Token{"-",lineNo,categoryType::MINUS});
        cursor++;
    }
    else if(srcCode[cursor] == '*'){
        if(cursor+1 < srcCode.size() && srcCode[cursor+1] == '='){
            tokens.push_back(Token{"*=",lineNo,categoryType::MULTIPLICATION_ASSIGNMENT});
            cursor+=2;
            return;
        }
        if(cursor+1 < srcCode.size() && srcCode[cursor+1] == '='){
            tokens.push_back(Token{"*=",lineNo,categoryType::MULTIPLICATION_ASSIGNMENT});
            cursor+=2;
            return;
        }
        tokens.push_back(Token{"*",lineNo,categoryType::MULTIPLY});
        cursor++;
    }
    else if(srcCode[cursor] == '/'){
        if(cursor+1 < srcCode.size() && srcCode[cursor+1] == '='){
            tokens.push_back(Token{"/=",lineNo,categoryType::DIVISION_ASSIGNMENT});
            cursor+=2;
            return;
        }
        if(cursor+1 < srcCode.size() && srcCode[cursor+1] == '='){
            tokens.push_back(Token{"/=",lineNo,categoryType::DIVISION_ASSIGNMENT});
            cursor+=2;
            return;
        }
        tokens.push_back(Token{"/",lineNo,categoryType::DIVIDE});
        cursor++;
    }
    else if(srcCode[cursor] == '='){
        if(cursor+1 >= srcCode.size())
            throw std::runtime_error("Lexer Error: operator at line "+ std::to_string(lineNo)+" not defined");
        if(srcCode[cursor+1] == '='){
            tokens.push_back(Token{"==",lineNo,categoryType::EQUAL_EQUAL});
            cursor+=2;
        }
        else
            throw std::runtime_error("Lexer Error: operator at line "+ std::to_string(lineNo)+" not defined");  
    }
    else 
        throw std::runtime_error("Lexer Error: Unknown operator at line " + std::to_string(lineNo));
    


}

void Lexer::scanLetter() {
    std::size_t letterStart = cursor;
    while(cursor<srcCode.size() && (std::isalpha(static_cast<unsigned char>(srcCode[cursor])) || srcCode[cursor]=='_' || std::isdigit(static_cast<unsigned char>(srcCode[cursor]))))
        cursor++;
    std::string org = srcCode.substr(letterStart, cursor-letterStart);
    tokens.push_back(Token{org, lineNo, categoryType::IDENTIFIER});
}



