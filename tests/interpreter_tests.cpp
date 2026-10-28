#include "Lexer.hpp"
#include "LexerError.hpp"
#include "AST.hpp"
#include "Parser.hpp"
#include "Evaluator.hpp"
#include "ParserError.hpp"
#include "Environment.hpp"

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <stdexcept>

std::vector<std::string> validSources = {
    "a := 10;",
    "a := 10 + 5 * 2;",
    "a := 10; b := a * 2;",
    "a := 10; a := 50;",
    "a := 10; b := a * 2; a := 50; c := a + b;",
    "x := (10 + 5) * 2;",
    "a := 100; b := a / 4; c := b - 5;",
    "a := 10;;; b := 20;;",
    "x := 3.5; y := x * 2;",
    "a := 10; b := a + a; c := b * a;"
};

std::vector<std::vector<double>> validExpectations = {
    {10},
    {20},
    {10, 20},
    {10, 50},
    {10, 20, 50, 70},
    {30},
    {100, 25, 20},
    {10, 20},
    {3.5, 7},
    {10, 20, 200}
};

bool validTester(){
    for(int i = 0 ; i < validSources.size(); i++){
        std::vector<std::unique_ptr<Expr>> valid_parsed_AST;

        Lexer lexer(validSources[i]);
        std::vector<Token> tokens = lexer.tokenize();

        Parser parser(tokens);
        parser.parseProgram(valid_parsed_AST);

        Environment env;
        std::vector<double> results;
        for(int j = 0 ; j < valid_parsed_AST.size() ; j++)
            results.push_back(evaluate(valid_parsed_AST[j].get(),env));
        if(results != validExpectations[i]){
            std::cout << "Failed test " << i+1 << ": " << validSources[i] << '\n';
            return false;
        }
    }
    return true;
}


std::vector<std::string> invalidSources = {
    "a := 10",
    "5 := 10;",
    "a := missing + 5;",
    "a := 10 / 0;",
    "a := 10; b := unknown * 2;",
    "a := 10 + * 5;",
    "a := 10; b := 20",
    "a := (10 + 5;",
    "a := 10; b := 20; c := b / (a - 10);",
    "a := 10; b := 20 c := 30;"
};

std::vector<std::string> invalidExpectations = {
    "Expected semicolon after expression",
    "Must have Identifier",
    "Identifier not accessible",
    "Division by zero is undefined",
    "Identifier not accessible",
    "Expected expression, found 'MULTIPLY'",
    "Expected semicolon after expression",
    "Unexpected Token'SEMICOLON'",
    "Division by zero is undefined",
    "Expected semicolon after expression"
};

std::vector<bool> expectsParserError = {
    true,
    true,
    false,
    false,
    false,
    true,
    true,
    true,
    false,
    true
};

bool invalidTester(){
    for(int i = 0 ; i < invalidSources.size() ; i++){
        try{
            std::vector<std::unique_ptr<Expr>> parsed_AST;
            Lexer lexer(invalidSources[i]);
            std::vector<Token> tokens = lexer.tokenize();
            Parser parser(tokens);
            parser.parseProgram(parsed_AST);

            Environment env;
            std::vector<double> results;
        
            for(int j = 0 ; j < parsed_AST.size() ; j++)
                results.push_back(evaluate(parsed_AST[j].get(),env));
            std::cout << "Invalid test " << i + 1 << " unexpectedly succeeded: " << invalidSources[i] << '\n';
            return false;
        }
        catch (const LexerError& e){
            std::cout << "Invalid test " << i + 1 << " Wrong error caught! Expected Lexer Error" << invalidSources[i] << '\n';
            return false;
        }

        catch (const ParserError& e){
            std::string message = e.what();
            if(message.find(invalidExpectations[i]) == std::string::npos || !expectsParserError[i]){
                std::cout << "Invalid test " << i + 1 << " Wrong error caught! Expected Parser Error " << invalidSources[i] << '\n';
                return false;
            }
        }
        catch(const std::runtime_error& e){
            std::string message = e.what();
            if(message.find(invalidExpectations[i]) == std::string::npos || expectsParserError[i]){
                std::cout << "Invalid test " << i + 1 << " Wrong error caught! Expected Runtime Error " << invalidSources[i] << '\n';
                return false;
            }
        }
    }
    return true;

}
    
int main(){
    if(validTester() && invalidTester()){
        std::cout<<"Passed";
        return 0;
    }
    else{
        std::cout<<"Failed";
        return 1;
    }
    
}