#include "Lexer.hpp"
#include "LexerError.hpp"
#include "AST.hpp"
#include "Parser.hpp"
#include "Evaluator.hpp"
#include "ParserError.hpp"
#include "Environment.hpp"
#include "Value.hpp"

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
    "manam a := 10; manam b := a + 5; b := b * 2;",
    "manam a := -15; manam b := -5; manam c := a + b;",
    "manam a := - -5; manam b := 10 * -2;",
    "manam a := -(5 + 3); manam b := 10 / -2;",
    "manam name := \"Rojan\"; manam country := \"Nepal\";",
    "manam first := \"Hello\"; manam second := \" World\"; manam full := first + second;",
    "manam a := \"Nepal\" == \"Nepal\"; manam b := \"Nepal\" != \"India\";",
    "manam a := -10; manam b := a < 0;",
    "manam a := 5; yedi a > 0 { a := 20; } a;",
    "manam a := 5; yedi a < 0 { a := 20; } natra { a := 30; } a;",
    "manam a := 60; manam result := 0; yedi a > 100 { result := 1; } navaye a > 50 { result := 2; } natra { result := 3; } result;",
    "manam a := 10; yedi a > 5 { yedi a == 10 { a := 50; } } a;"
};

std::vector<std::vector<Value>> validExpectations = {
    {10.0},
    {20.0},
    {10.0, 20.0},
    {10.0, 50.0},
    {10.0, 20.0, 50.0, 70.0},
    {30.0},
    {100.0, 25.0, 20.0},
    {10.0, 20.0},
    {3.5, 7.0},
    {10.0, 20.0, 200.0},
    {5.0, 15.0, 30.0},
    {10.0, 15.0, 30.0},
    {-15.0, -5.0, -20.0},
    {5.0, -20.0},
    {-8.0, -5.0},
    {std::string("Rojan"), std::string("Nepal")},
    {std::string("Hello"), std::string(" World"), std::string("Hello World")},
    {1.0, 1.0},
    {-10.0, 1.0},
    {5.0, 0.0, 20.0},
    {5.0, 0.0, 30.0},
    {60.0, 0.0, 0.0, 2.0},
    {10.0, 0.0, 50.0}
};

bool validTester(){
    if(validSources.size() != validExpectations.size())
        throw std::runtime_error("Valid test data size mismatch");
    for(int i = 0 ; i < validSources.size(); i++){
        std::vector<std::unique_ptr<Expr>> valid_parsed_AST;

        Lexer lexer(validSources[i]);
        std::vector<Token> tokens = lexer.tokenize();

        Parser parser(tokens);
        parser.parseProgram(valid_parsed_AST);

        Environment env;
        std::vector<Value> results;
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
    "manam a := 10; b := a + 5;",
    "manam a := 10 + \"Nepal\";",
    "manam a := \"Hello\" * \"World\";",
    "manam a := \"Hello\" - \"World\";",
    "manam a := 10; yedi \"hello\" { a := 20; }",
    "manam a := \"hello\"; manam b := a / 2;",
    "manam a := 10 / (5 - 5);",
    "manam a := -;",
    "manam a := 10; navaye a > 5 { a := 20; }",
    "manam a := 10; natra { a := 20; }",
    "manam a := 10; yedi a > 5 { a := 20;"
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
    "Identifier not declared",
    "Incompatible operand types",
    "Unsupported operator with strings",
    "Unsupported operator with strings",
    "Condition must be numeric",
    "Incompatible operand types",
    "Division by zero is undefined",
    "Expected expression, found 'SEMICOLON'",
    "Expected expression",
    "Expected expression",
    "Expected }"
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
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    true,
    true,
    true,
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
            std::vector<Value> results;
        
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