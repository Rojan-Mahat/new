#pragma once

#include "Token.hpp"
#include "Value.hpp"
#include<memory>
#include<utility>
#include<vector>
#include<string>

struct Expr{
    virtual ~Expr() = default;
};

struct LiteralNode : Expr{
    Value value;
    LiteralNode(Value valueFromToken)
        :value(std::move(valueFromToken))
    {}
};

struct BinaryNode : Expr{
    categoryType op;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;

    BinaryNode(categoryType type , std::unique_ptr<Expr> p1 , std::unique_ptr<Expr> p2)
        :op(type),
        left(std::move(p1)),
        right(std::move(p2))
        {}
};


struct IdentifierNode : Expr{
    std::string name;
    IdentifierNode(std::string nameFromToken)
        :name(std::move(nameFromToken))
    {}
};

struct AssignmentNode : Expr{
    std::string name;
    std::unique_ptr<Expr> right;

    AssignmentNode(std::string nameFromToken, std::unique_ptr<Expr> ptr)
        :name(std::move(nameFromToken)),
        right(std::move(ptr))
        {}
};

struct DeclarationNode : Expr {
    std::string name;
    std::unique_ptr<Expr> initializer;

    DeclarationNode(std::string nameFromToken, std::unique_ptr<Expr> ptr)
        :name(std::move(nameFromToken)),
        initializer(std::move(ptr))
        {}
};

struct YediNode : Expr{
    std::unique_ptr<Expr> conditional_statement;
    std::vector<std::unique_ptr<Expr>> body_statements;
    std::unique_ptr<Expr> alt;

    YediNode(std::unique_ptr<Expr> condition,std::vector<std::unique_ptr<Expr>> body)
        :conditional_statement(std::move(condition)),
        body_statements(std::move(body)),
        alt(nullptr)
    {}

    YediNode(std::unique_ptr<Expr> condition,std::vector<std::unique_ptr<Expr>> body, std::unique_ptr<Expr> alternative)
        :conditional_statement(std::move(condition)),
        body_statements(std::move(body)),
        alt(std::move(alternative))
    {}
};

struct NatraNode : Expr{
    std::vector<std::unique_ptr<Expr>> body_statements;
    NatraNode(std::vector<std::unique_ptr<Expr>> body)
        :body_statements(std::move(body))
    {}
};

struct StringNode : Expr {
    std::string value;
    StringNode(std::string valueFromToken)
        :value(valueFromToken)
    {}
};

struct ChapaNode : Expr{
    std::unique_ptr<Expr> expression;
    ChapaNode(std::unique_ptr<Expr> expr)
        :expression(std::move(expr))
    {}
};

struct JabasammaNode : Expr{
    std::unique_ptr<Expr> conditional_statement;
    std::vector<std::unique_ptr<Expr>> body_statements;
    
    JabasammaNode(std::unique_ptr<Expr> condition,std::vector<std::unique_ptr<Expr>> body)
        :conditional_statement(std::move(condition)),
        body_statements(std::move(body))
    {}

};

struct KaamNode : Expr{
    std::string name;
    std::vector<std::string> parameters;
    std::vector<std::unique_ptr<Expr>> body_statements;

    KaamNode(std::string functioname, std::vector<std::string> names,std::vector<std::unique_ptr<Expr>> body)
        :name(std::move(functioname)),
        parameters(std::move(names)),
        body_statements(std::move(body))
    {}
};

struct KaamGarNode : Expr{
    std::unique_ptr<Expr> function;
    std::vector<std::unique_ptr<Expr>> arguments;

    KaamGarNode(std::unique_ptr<Expr> name,std::vector<std::unique_ptr<Expr>> args)
        :function(std::move(name)),
        arguments(std::move(args))
    {}
};

struct FirtaNode : Expr{
    std::unique_ptr<Expr> value;

    FirtaNode(std::unique_ptr<Expr> expr = nullptr)
        :value(std::move(expr))
    {}
};