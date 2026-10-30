#pragma once
#include <unordered_map>
#include <string>
class Environment{
    private:
    std::unordered_map<std::string, double> variables;

    public:
    void storeVariables(std::string, double data);
    double getVariables(std::string);
    bool ifExists(std::string);
    void declareVariables(std::string, double data);
};