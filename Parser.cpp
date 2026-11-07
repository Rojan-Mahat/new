#include"Parser.hpp"
#include "ParserError.hpp"
#include "Value.hpp"
#include<stdexcept>

bool Parser::isBlockStatement(categoryType type){
    return type == categoryType::YEDI || type == categoryType::JABASAMMA ||type == categoryType::KAAM;
}

void Parser::parseProgram(std::vector<std::unique_ptr<Expr>>& vec){
    while(cursor < tokens.size()){
        auto ptr = parse();
        vec.push_back(std::move(ptr));
    }
}

std::unique_ptr<Expr> Parser::parse(){
    bool isBlock = cursor < tokens.size() && isBlockStatement(tokens[cursor].type);
    auto root = parseStatement();
    if(isBlock)
        return root;
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
    if(cursor < tokens.size() && tokens[cursor].type == categoryType::KAAMGAR)
        return parseKaamGar();
    
    if(cursor < tokens.size() && tokens[cursor].type == categoryType::IDENTIFIER){
        auto ptr = std::make_unique<IdentifierNode>(tokens[cursor].original);
        cursor++;
        return ptr;
    }

    if(cursor < tokens.size() && tokens[cursor].type == categoryType::NUMBER){
        double num = std::stod(tokens[cursor].original);
        auto ptr = std::make_unique<LiteralNode>(num);
        cursor++;
        return ptr;
    }

    if(cursor < tokens.size() && tokens[cursor].type == categoryType::STRING){
        auto ptr = std::make_unique<LiteralNode>(tokens[cursor].original);
        cursor++;
        return ptr;
    }

    if(cursor < tokens.size() && tokens[cursor].type == categoryType::LEFT_PAREN){
        int openingline = tokens[cursor].line;
        cursor++;
        auto subtree = parseComparison();
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
    auto left = parseUnary();
    while(cursor < tokens.size() && (tokens[cursor].type == categoryType::MULTIPLY || tokens[cursor].type == categoryType::DIVIDE)){
        categoryType op = tokens[cursor].type;
        cursor++;
        auto right = parseUnary();
        left = std::make_unique<BinaryNode>(op,std::move(left),std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::parseUnary(){
    if(cursor >= tokens.size() || tokens[cursor].type != categoryType::MINUS)
        return parsePrimary();
    cursor++;
    auto innerUnary = parseUnary();
    return std::make_unique<BinaryNode>(categoryType::MINUS, 
            std::make_unique<LiteralNode>(0.0),
            std::move(innerUnary));
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
    if(cursor < tokens.size() && tokens[cursor].type == categoryType::KAAM)
        return parseKaam();
    if(cursor < tokens.size() && tokens[cursor].type == categoryType::JABASAMMA)
        return parseJabasamma();
    if(cursor < tokens.size() && tokens[cursor].type == categoryType::CHAPA)
        return parseChapa();
    if(cursor < tokens.size() && tokens[cursor].type == categoryType::YEDI)
        return parseYedi();
    if(cursor < tokens.size() && tokens[cursor].type == categoryType::MANAM)
        return parseDeclaration();
    return parseAssignment();
}

std::unique_ptr<Expr> Parser::parseKaam(){
    cursor++;
    if(cursor >= tokens.size()) 
        throw ParserError("Error: Statement closed unexpectedly", tokens.empty() ? 1 : tokens.back().line);
    if(tokens[cursor].type != categoryType::IDENTIFIER)
        throw ParserError("Error: Unexpected statement, Expected function name", tokens[cursor].line);
    
    std::string functionName = tokens[cursor].original;
    cursor++;

    if(cursor >= tokens.size()) 
        throw ParserError("Error: Statement closed unexpectedly", tokens.empty() ? 1 : tokens.back().line);
    if(tokens[cursor].type != categoryType::LEFT_PAREN)
        throw ParserError("Error: Unexpected statement, Expected (", tokens[cursor].line);
    
    cursor++;
    std::vector<std::string> parameters;
    while(cursor < tokens.size() && tokens[cursor].type != categoryType::RIGHT_PAREN){
        if(tokens[cursor].type == categoryType::IDENTIFIER)
            parameters.push_back(tokens[cursor].original);
        else if(tokens[cursor].type != categoryType::COMMA)
            throw ParserError("Invalid parameter", tokens[cursor].line);
        cursor++;
    }
    
    if(cursor >= tokens.size()) 
        throw ParserError("Error: Statement closed unexpectedly", tokens.empty() ? 1 : tokens.back().line);

    cursor++;
    if(cursor >= tokens.size()) 
        throw ParserError("Error: Statement closed unexpectedly", tokens.empty() ? 1 : tokens.back().line);
    if(tokens[cursor].type != categoryType::LEFT_BRACE)
        throw ParserError("Error: Unexpected statement, Expected {", tokens[cursor].line);

    cursor++;
    std::vector<std::unique_ptr<Expr>> body;
    while(cursor < tokens.size() && tokens[cursor].type != categoryType::RIGHT_BRACE){
        body.push_back(parse());
    }

    if(cursor >= tokens.size())
        throw ParserError("Error: Statement closed unexpectedly, Expected }", tokens.empty() ? 1 : tokens.back().line);
    if(tokens[cursor].type != categoryType::RIGHT_BRACE)
        throw ParserError("Error: Unexpected statement, Expected }", tokens[cursor].line);
    
    cursor++;
    return std::make_unique<KaamNode>(std::move(functionName), std::move(parameters), std::move(body)); 
}

std::unique_ptr<Expr> Parser::parseKaamGar(){
    cursor++;
    if(cursor >= tokens.size()) 
        throw ParserError("Error: Statement closed unexpectedly", tokens.empty() ? 1 : tokens.back().line);
    if(tokens[cursor].type != categoryType::LEFT_PAREN)
        throw ParserError("Error: Unexpected statement, Expected (", tokens[cursor].line);
    
    cursor++;
    if(cursor >= tokens.size()) 
        throw ParserError("Error: Statement closed unexpectedly", tokens.empty() ? 1 : tokens.back().line);
    if(tokens[cursor].type != categoryType::IDENTIFIER)
        throw ParserError("Error: Unexpected statement, Expected function call", tokens[cursor].line);

    std::unique_ptr<Expr> function = parsePrimary();

    if(cursor >= tokens.size()) 
        throw ParserError("Error: Statement closed unexpectedly", tokens.empty() ? 1 : tokens.back().line);
    if(tokens[cursor].type != categoryType::LEFT_PAREN)
        throw ParserError("Error: Unexpected statement, Expected (", tokens[cursor].line);
    
    cursor++;
    std::vector<std::unique_ptr<Expr>> arguments;
    while(cursor < tokens.size() && tokens[cursor].type != categoryType::RIGHT_PAREN){
        arguments.push_back(parseComparison());
        if(cursor >= tokens.size())
            throw ParserError("Expected comma or )", tokens.back().line);

        if(tokens[cursor].type == categoryType::COMMA){
            cursor++;
            if (cursor >= tokens.size() || tokens[cursor].type == categoryType::RIGHT_PAREN)
                throw ParserError("Expected argument after comma", tokens[cursor - 1].line);
        }

        else if (tokens[cursor].type != categoryType::RIGHT_PAREN) 
            throw ParserError("Expected comma or )", tokens[cursor].line);
        
    }

    if(cursor >= tokens.size()) 
        throw ParserError("Error: Statement closed unexpectedly", tokens.empty() ? 1 : tokens.back().line);
    
    cursor++;

    if(cursor >= tokens.size()) 
        throw ParserError("Error: Statement closed unexpectedly", tokens.empty() ? 1 : tokens.back().line);
    if(tokens[cursor].type != categoryType::RIGHT_PAREN)
        throw ParserError("Error: Unexpected statement, Expected )", tokens[cursor].line);
    
    cursor++;
    return std::make_unique<KaamGarNode>(std::move(function), std::move(arguments));
}

std::unique_ptr<Expr> Parser::parseJabasamma(){
    cursor++;
    auto comparisions = parseComparison();
    if(cursor >= tokens.size()) 
        throw ParserError("Error: Statement closed unexpectedly", tokens.empty() ? 1 : tokens.back().line);
    if(tokens[cursor].type != categoryType::LEFT_BRACE)
        throw ParserError("Error: Unexpected statement, Expected {", tokens[cursor].line);
    cursor++;
    std::vector<std::unique_ptr<Expr>> body;
    while(cursor < tokens.size() && tokens[cursor].type != categoryType::RIGHT_BRACE){
        body.push_back(parse());
    }
    if(cursor >= tokens.size()) 
        throw ParserError("Error: Statement closed unexpectedly, Expected }", tokens.empty() ? 1 : tokens.back().line);
    if(tokens[cursor].type != categoryType::RIGHT_BRACE)
        throw ParserError("Error: Expected }", tokens[cursor].line);
    cursor++;
    return std::make_unique<JabasammaNode>(std::move(comparisions), std::move(body));
}


std::unique_ptr<Expr> Parser::parseYedi(){
    if(cursor >= tokens.size()) 
        throw ParserError("Error: Statement closed unexpectedly", tokens.empty() ? 1 : tokens.back().line);

    categoryType currentType = tokens[cursor].type;
    if(currentType != categoryType::YEDI && currentType != categoryType::NAVAYE && currentType != categoryType::NATRA)
        throw ParserError("Error: Unexpected statement, Expected conditional", tokens[cursor].line);
        
    cursor++;
    std::unique_ptr<Expr> conditions;
    if(currentType != categoryType::NATRA)
        conditions = parseComparison();
    if(cursor >= tokens.size())
        throw ParserError("Error: Statement closed unexpectedly, Expected {", tokens.empty() ? 1 : tokens.back().line);
    if(tokens[cursor].type != categoryType::LEFT_BRACE)
        throw ParserError("Error: Unexpected statement, Expected {", tokens[cursor].line);

    cursor++;
    std::vector<std::unique_ptr<Expr>> body;
    while(cursor < tokens.size() && tokens[cursor].type != categoryType::RIGHT_BRACE){
        body.push_back(parse());
    }

    if(cursor >= tokens.size())
        throw ParserError("Error: Statement closed unexpectedly, Expected }", tokens.empty() ? 1 : tokens.back().line);
    if(tokens[cursor].type != categoryType::RIGHT_BRACE)
        throw ParserError("Error: Unexpected statement, Expected }", tokens[cursor].line);

    cursor++;
    if(currentType == categoryType::NATRA){
        return std::make_unique<NatraNode>(std::move(body));
    }

    if(cursor < tokens.size() && (tokens[cursor].type == categoryType::NAVAYE || tokens[cursor].type == categoryType::NATRA)){
        auto innerPtr = parseYedi();
        return std::make_unique<YediNode> (std::move(conditions),std::move(body),std::move(innerPtr));
    }
    else
        return std::make_unique<YediNode>(std::move(conditions), std::move(body));
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
    auto right = parseComparison();
    auto ptr = std::make_unique<DeclarationNode>(name,std::move(right));
    return ptr;
}

std::unique_ptr<Expr> Parser::parseAssignment(){
    auto left = parseComparison();
    if(cursor >= tokens.size() || tokens[cursor].type != categoryType::ASSIGNMENT)
        return left;

    auto IN = dynamic_cast<const IdentifierNode*>(left.get());
    if(IN == nullptr)
        throw ParserError("Must have Identifier", tokens[cursor].line);

    cursor++;
    auto right = parseComparison();
    auto ptr = std::make_unique<AssignmentNode>(IN->name,std::move(right));
    return ptr;
}

std::unique_ptr<Expr> Parser::parseComparison(){
    auto left = parseExpression();
    while(cursor < tokens.size() && (tokens[cursor].type == categoryType::EQUAL_EQUAL
    || tokens[cursor].type == categoryType::GREATER_THAN
    || tokens[cursor].type == categoryType::NOT_EQUAL
    || tokens[cursor].type == categoryType::GREATER_OR_EQUAL_TO
    || tokens[cursor].type == categoryType::SMALLER_THAN
    || tokens[cursor].type == categoryType::SMALLER_OR_EQUAL_TO)){
        auto op = tokens[cursor].type;
        cursor++;
        auto right = parseExpression();
        left = std::make_unique<BinaryNode>(op,std::move(left),std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::parseChapa(){
    cursor++;
    auto expr = parseComparison();
    return std::make_unique<ChapaNode>(std::move(expr));
}

