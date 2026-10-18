#pragma once

#include<vector>
#include<memory>
#include"AST.hpp"



struct Parser{
    private:
        std::vector<Token> tokens;
        std::size_t cursor;
        std::unique_ptr<Expr> parsePrimary();
        std::unique_ptr<Expr> parseTerm();
        std::unique_ptr<Expr> parseExpression();
    

    public:
    Parser(const std::vector<Token>& lexerTokens)
        :tokens(lexerTokens),
        cursor(0)
        {}
    
    std::unique_ptr<Expr> parse();
    
};
