#include "Environment.hpp"
#include <stdexcept>

void Environment::storeVariables(std::string name, double data){
    variables[name] = data;
}

bool Environment::ifExists(std::string name){
    auto it = variables.find(name);
    return it != variables.end();
}

double Environment::getVariables(std::string name){
    auto it = variables.find(name);
    if(it != variables.end()){
        return it->second;;
    }
    throw std::runtime_error("Identifier not accessible");
}