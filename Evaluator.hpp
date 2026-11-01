#pragma once
#include "AST.hpp"
#include "Parser.hpp"
#include "Environment.hpp"
#include "Value.hpp"


Value evaluate(const Expr* node , Environment& env);