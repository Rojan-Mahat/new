#pragma once
#include <unordered_map>
#include <string>
#include "Value.hpp"
class Environment{
    private:
    std::unordered_map<std::string, Value> variables;

    public:
    void storeVariables(std::string, Value data);
    Value getVariables(std::string);
    bool ifExists(std::string);
    void declareVariables(std::string, Value data);
};