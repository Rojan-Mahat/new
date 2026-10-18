#include"Parser.hpp"

#include<stdexcept>

std::unique_ptr<Expr> Parser::parsePrimary(){
    if(cursor < tokens.size() && tokens[cursor].type == categoryType::NUMBER){
        Token currentToken = tokens[cursor];
        double num = std::stod(currentToken.original);
        auto ptr = std::make_unique<NumberNode>(num);
        cursor++;
        return ptr;
    }

    if(cursor < tokens.size() && tokens[cursor].type == categoryType::LEFT_PAREN){
        std::string openingline = std::to_string(tokens[cursor].line);
        cursor++;
        auto subtree = parseExpression();
        if(cursor >= tokens.size() || tokens[cursor].type != categoryType::RIGHT_PAREN)
            throw std::runtime_error("Parenthesis not closed! Opened at line " + openingline);
        cursor++;
        return subtree;
    }

    throw std::runtime_error("Numbers expected!");
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
        throw std::runtime_error("Parser Error: parser bug");
    return root;
}