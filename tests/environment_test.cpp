#include "Environment.hpp"
#include <iostream>

int main(){
    Environment env;
    env.declareVariables("a", 5.0);
    env.declareVariables("b", 50.0);
    try{
        env.getVariables("b");
    }
    catch(const std::runtime_error& e){
        std::cout<< e.what();
    }
}