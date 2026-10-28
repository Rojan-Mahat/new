#include "Environment.hpp"
#include <stdexcept>

void Environment::storeVariables(std::string name, double data){
    variables[name] = data;
}

double Environment::getVariables(std::string name){
    auto it = variables.find(name);
    if(it != variables.end()){
        return it->second;;
    }
    throw std::runtime_error("Identifier not accesible");
}