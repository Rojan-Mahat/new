#pragma once
#include "AST.hpp"
#include "Parser.hpp"
#include "Environment.hpp"
#include "Value.hpp"
#include "FunctionValue.hpp"


Value evaluate(const Expr* node , Environment& env);