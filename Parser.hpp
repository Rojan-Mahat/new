#pragma once

#include<vector>
#include<memory>
#include"AST.hpp"



struct Parser{
    private:
        std::vector<Token> tokens;
        std::size_t cursor;
        std::unique_ptr<Expr> parseStatement();

        std::unique_ptr<Expr> parseKaam();//parseComparision
        std::unique_ptr<Expr> parseJabasamma();//parseComparision
        std::unique_ptr<Expr> parseChapa();//parseComparision
        std::unique_ptr<Expr> parseDeclaration();//parseComparision
        std::unique_ptr<Expr> parseYedi();
        //parseComparision[condition], parse()[each body], parseYedi[navaye/natra]
        std::unique_ptr<Expr> parseAssignment();
        //parseComparison(),parseExpression(),parseTerm(),parseUnary(),parsePrimary()
        std::unique_ptr<Expr> parseFirta();

        std::unique_ptr<Expr> parseComparison();
        std::unique_ptr<Expr> parseExpression();
        std::unique_ptr<Expr> parseTerm();
        std::unique_ptr<Expr> parseUnary();
        std::unique_ptr<Expr> parseKaamGar();
        std::unique_ptr<Expr> parsePrimary();
       
        

        bool isBlockStatement(categoryType type);
       
    

    public:
    Parser(const std::vector<Token>& lexerTokens)
        :tokens(lexerTokens),
        cursor(0)
        {}
    
    void parseProgram(std::vector<std::unique_ptr<Expr>>& vec);
    std::unique_ptr<Expr> parse();
    
};
