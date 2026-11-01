#include "Environment.hpp"
#include <stdexcept>

void Environment::storeVariables(std::string name, Value data){
    if(!ifExists(name))
        throw std::runtime_error("Identifier not declared");
    variables[name] = std::move(data);
}

void Environment::declareVariables(std::string name, Value data){
    if(ifExists(name))
        throw std::runtime_error("Identifier already declared");
    variables[name] = std::move(data);
}

bool Environment::ifExists(std::string name){
    auto it = variables.find(name);
    return it != variables.end();
}

Value Environment::getVariables(std::string name){
    auto it = variables.find(name);
    if(it != variables.end()){
        return it->second;
    }
    throw std::runtime_error("Identifier not accessible");
}