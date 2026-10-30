#pragma once

#include<memory>
#include<utility>
#include "Token.hpp"
#include<vector>

struct Expr{
    virtual ~Expr() = default;
};

struct NumberNode : Expr{
    double value;
    NumberNode(double num)
        :value(num)
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

    YediNode(std::unique_ptr<Expr> condition,std::vector<std::unique_ptr<Expr>> body)
        :conditional_statement(std::move(condition)),
        body_statements(std::move(body))
    {}
};