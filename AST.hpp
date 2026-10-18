#pragma once

#include<memory>
#include<utility>
#include "Token.hpp"

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