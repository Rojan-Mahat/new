#include<iostream>
#include <stdexcept>
#include <fstream>
#include<vector>

#include"Lexer.hpp"
#include"LexerError.hpp"
#include"AST.hpp"
#include "Parser.hpp"

std::string readSourceCode(std::ifstream& file){
    std::string src;
    char ch;
    if(!file.is_open())
        throw std::runtime_error("Source code could not be opened");
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

        Parser parser(tokens);
        auto root = parser.parse();
        std::cout << "Parsing succeeded!\n";

        auto* binary = dynamic_cast<BinaryNode*>(root.get());
        if (binary && binary->op == categoryType::PLUS) 
            std::cout << "Root operator is PLUS!\n";
    }

    catch (const LexerError& e){
        std::cerr << e.what() << " at line " << e.getLine() << '\n';
        return 1;
    }
    catch(const std::runtime_error& e){
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    
    
    return 0;
}