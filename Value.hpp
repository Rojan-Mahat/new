#pragma once

#include <variant>
#include <string>
#include "FunctionValue.hpp"

using Value = std::variant<double, std::string , FunctionValue>;