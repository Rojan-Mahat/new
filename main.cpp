#include<iostream>
#include <stdexcept>
#include <fstream>
#include<vector>
#include <cctype>

#include"Lexer.hpp"


std::string readSourceCode(std::ifstream& file){
    std::string src;
    char ch;
    if(!file.is_open())
        throw std::runtime_error("source code could not be opened");
    while(file.get(ch))
       src.push_back(ch);
    return src;
}

int main(){
    try{
        std::ifstream file("test.txt");
        std::string source = readSourceCode(file);
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.tokenize();

        for (const Token& token : tokens) 
            std::cout << token.original << " | line " << token.line << " type " << tokenTypeToString(token.type) <<'\n';
    }
    catch(const std::runtime_error& e){
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    
    return 0;
}