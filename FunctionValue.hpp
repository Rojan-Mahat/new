#pragma once
struct KaamNode;
class Environment;

struct FunctionValue{
    const KaamNode* definition = nullptr;
    Environment* definedEnv = nullptr;
};
       
