#include"Parser.hpp"
#include "ParserError.hpp"
#include<stdexcept>

std::unique_ptr<Expr> Parser::parsePrimary(){
    if(cursor < tokens.size() && tokens[cursor].type == categoryType::IDENTIFIER){
        auto ptr = std::make_unique<IdentifierNode>(tokens[cursor].original);
        cursor++;
        return ptr;
    }
    
    if(cursor < tokens.size() && tokens[cursor].type == categoryType::NUMBER){
        double num = std::stod(tokens[cursor].original);
        auto ptr = std::make_unique<NumberNode>(num);
        cursor++;
        return ptr;
    }

    if(cursor < tokens.size() && tokens[cursor].type == categoryType::LEFT_PAREN){
        int openingline = tokens[cursor].line;
        cursor++;
        auto subtree = parseExpression();
        if(cursor >= tokens.size())
            throw ParserError("Parenthesis not closed! Opened at line ", openingline);
        if(tokens[cursor].type != categoryType::RIGHT_PAREN)
            throw ParserError(("Unexpected Token'"+tokenTypeToString(tokens[cursor].type)+"' "), tokens[cursor].line);
        cursor++;
        return subtree;
    }

    if(cursor >= tokens.size()){
        int line = tokens.empty() ? 1 : tokens.back().line;
        throw ParserError("Expected expression, reached end of input" , line);
    }
    throw ParserError(("Expected expression, found '"+tokenTypeToString(tokens[cursor].type)+"'"),tokens[cursor].line);
}

std::unique_ptr<Expr> Parser::parseTerm(){
    auto left = parsePrimary();
    while(cursor < tokens.size() && 
    (tokens[cursor].type == categoryType::MULTIPLY || 
    tokens[cursor].type == categoryType::DIVIDE))
    {
        categoryType op = tokens[cursor].type;
        cursor++;
        auto right = parsePrimary();
        left = std::make_unique<BinaryNode>(op,std::move(left),std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::parseExpression(){
    auto left = parseTerm();
    while(cursor < tokens.size() && 
    (tokens[cursor].type == categoryType::PLUS || 
    tokens[cursor].type == categoryType::MINUS)){
        categoryType op = tokens[cursor].type;
        cursor++;
        auto right = parseTerm();
        left = std::make_unique<BinaryNode>(op,std::move(left),std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::parse(){
    auto root = parseExpression();
    if(cursor != tokens.size())
        throw ParserError("Unexpected token '" +tokenTypeToString(tokens[cursor].type)+"' after expression",tokens[cursor].line);
    return root;
}