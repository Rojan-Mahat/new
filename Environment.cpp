#include "Environment.hpp"
#include <stdexcept>

void Environment::storeVariables(std::string name, double data){
    if(!ifExists(name))
        throw std::runtime_error("Identifier not declared");
    variables[name] = data;
}

void Environment::declareVariables(std::string name, double data){
    if(ifExists(name))
        throw std::runtime_error("Identifier already declared");
    variables[name] = data;
}


bool Environment::ifExists(std::string name){
    auto it = variables.find(name);
    return it != variables.end();
}

double Environment::getVariables(std::string name){
    auto it = variables.find(name);
    if(it != variables.end()){
        return it->second;
    }
    throw std::runtime_error("Identifier not accessible");
}