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
    "manam a := 10;",
    "manam a := 10 + 5 * 2;",
    "manam a := 10; manam b := a * 2;",
    "manam a := 10; a := 50;",
    "manam a := 10; manam b := a * 2; a := 50; manam c := a + b;",
    "manam x := (10 + 5) * 2;",
    "manam a := 100; manam b := a / 4; manam c := b - 5;",
    "manam a := 10;;; manam b := 20;;",
    "manam x := 3.5; manam y := x * 2;",
    "manam a := 10; manam b := a + a; manam c := b * a;",
    "manam a := 5; a := a + 10; a := a * 2;",
    "manam a := 10; manam b := a + 5; b := b * 2;"
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
    {10, 20, 200},
    {5, 15, 30},
    {10, 15, 30}
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
    "manam a := 10",
    "manam 5 := 10;",
    "manam a := missing + 5;",
    "manam a := 10 / 0;",
    "manam a:= 10; manam b := unknown * 2;",
    "manam a := 10 + * 5;",
    "manam a := 10; manam b := 20",
    "manam a := (10 + 5;",
    "manam a := 10; manam b := 20; manam c := b / (a - 10);",
    "manam a := 10; manam b := 20 manam c := 30;",
    "a := 10;",
    "manam a := 10; manam a := 20;",
    "manam a :=;",
    "manam a 10;",
    "manam := 10;",
    "manam a;",
    "manam a := 10; b := a + 5;"
};

std::vector<std::string> invalidExpectations = {
    "Expected semicolon after expression",
    "Expected identifier",
    "Identifier not accessible",
    "Division by zero is undefined",
    "Identifier not accessible",
    "Expected expression, found 'MULTIPLY'",
    "Expected semicolon after expression",
    "Unexpected Token'SEMICOLON'",
    "Division by zero is undefined",
    "Expected semicolon after expression",
    "Identifier not declared",
    "Identifier already declared",
    "Expected expression, found 'SEMICOLON'",
    "Expected :=",
    "Expected identifier",
    "Expected :=",
    "Identifier not declared"
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
    true,
    false,
    false,
    true,
    true,
    true,
    true,
    false
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