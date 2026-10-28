#pragma once
#include "AST.hpp"
#include "Parser.hpp"
#include "Environment.hpp"


double evaluate(const Expr* node , Environment& env);