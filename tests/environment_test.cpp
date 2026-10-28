#include "Environment.hpp"
#include <iostream>

int main(){
    Environment env;
    env.storeVariables("a", 5.0);
    std::cout << env.getVariables("a")<<std::endl;
    env.storeVariables("a", 50);
    std::cout << env.getVariables("a")<<std::endl;
    try{
        std::cout << env.getVariables("b");
    }
    catch(const std::runtime_error& e){
        std::cout<< e.what();
    }
}