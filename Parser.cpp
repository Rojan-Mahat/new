#include"Parser.hpp"
#include "ParserError.hpp"
#include<stdexcept>


void Parser::parseProgram(std::vector<std::unique_ptr<Expr>>& vec){
    while(cursor < tokens.size()){
        auto ptr = parse();
        vec.push_back(std::move(ptr));
    }
}

std::unique_ptr<Expr> Parser::parse(){
    auto root = parseStatement();
    int line = tokens.empty() ? 1 : tokens.back().line;
    if(cursor >= tokens.size())
        throw ParserError("Expected semicolon after expression", line);
    if(tokens[cursor].type != categoryType::SEMICOLON)
        throw ParserError("Expected semicolon after expression", tokens[cursor].line);

    while(cursor < tokens.size() && tokens[cursor].type == categoryType::SEMICOLON)
        cursor++;
    return root;
}

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

std::unique_ptr<Expr> Parser::parseStatement(){
    if(cursor < tokens.size() && tokens[cursor].type == categoryType::MAKE)
        return parseDeclaration();
    return parseAssignment();
}

std::unique_ptr<Expr> Parser::parseDeclaration(){
    cursor++;
    if(cursor >= tokens.size())
        throw ParserError("Statement closed unexpectedly, Expected identifier", tokens.empty() ? 1 : tokens.back().line);
    if(tokens[cursor].type != categoryType::IDENTIFIER)
        throw ParserError("Expected identifier", tokens[cursor].line);

    std::string name = tokens[cursor].original;

    cursor++;
    if(cursor >= tokens.size())
        throw ParserError("Statement closed unexpectedly, Expected :=", tokens.empty() ? 1 : tokens.back().line);
    if(tokens[cursor].type != categoryType::ASSIGNMENT)
        throw ParserError("Expected :=", tokens[cursor].line);

    cursor++;
    auto right = parseExpression();
    auto ptr = std::make_unique<DeclarationNode>(name,std::move(right));
    return ptr;
}

std::unique_ptr<Expr> Parser::parseAssignment(){
    auto left = parseExpression();
    if(cursor >= tokens.size() || tokens[cursor].type != categoryType::ASSIGNMENT)
        return left;

    auto IN = dynamic_cast<const IdentifierNode*>(left.get());
    if(IN == nullptr)
        throw ParserError("Must have Identifier", tokens[cursor].line);

    cursor++;
    auto right = parseExpression();
    auto ptr = std::make_unique<AssignmentNode>(IN->name,std::move(right));
    return ptr;
}




