#include <iostream>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <memory>

#include "Lexer.hpp"
#include "LexerError.hpp"
#include "AST.hpp"
#include "Parser.hpp"
#include "Evaluator.hpp"
#include "ParserError.hpp"
#include "Environment.hpp"
#include "Value.hpp"

std::vector<std::unique_ptr<Expr>> parsed_AST;
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
        parser.parseProgram(parsed_AST);

        Environment env;
        for(const std::unique_ptr<Expr>& root : parsed_AST)
            const Value result = evaluate(root.get(),env);
    }

    catch (const LexerError& e){
        std::cerr << e.what() << " at line " << e.getLine() << '\n';
        return 1;
    }

    catch (const ParserError& e){
        std::cerr << e.what() << " at line " << e.getLine() << '\n';
        return 1;
    }
    catch(const std::runtime_error& e){
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }


    
    
    return 0;
}