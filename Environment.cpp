#include "Environment.hpp"
#include <stdexcept>

void Environment::storeVariables(std::string name, Value data){
    if(!ifExists(name)){
        if(ifParentExists()){
            parent->storeVariables(name,data);
            return;
        }
        else
            throw std::runtime_error("Identifier not declared");
    }
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

bool Environment::ifParentExists(){
    return parent != nullptr;
}

Value Environment::getVariables(std::string name){
    auto currentMap = variables.find(name);
    if(currentMap != variables.end()){
        return currentMap->second;
    }
    if(ifParentExists())
        return parent->getVariables(name);
    throw std::runtime_error("Unknown identifer found");
} 

