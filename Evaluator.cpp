#include "Evaluator.hpp"
#include "Environment.hpp"
#include "ReturnSignal.hpp"
#include<iostream>

Value evaluate(const Expr* node, Environment& env){
    const LiteralNode* LN = dynamic_cast <const LiteralNode*> (node);
    const BinaryNode* BN = dynamic_cast<const BinaryNode*> (node);
    const IdentifierNode* IN = dynamic_cast<const IdentifierNode*> (node);
    const AssignmentNode* AN = dynamic_cast<const AssignmentNode*> (node);
    const DeclarationNode* DN = dynamic_cast<const DeclarationNode*> (node);
    const YediNode* YN = dynamic_cast<const YediNode*> (node);
    const ChapaNode* CN = dynamic_cast<const ChapaNode*> (node);
    const JabasammaNode* JN = dynamic_cast<const JabasammaNode*> (node);
    const KaamNode* KN = dynamic_cast<const KaamNode*> (node);
    const KaamGarNode* KGN = dynamic_cast<const KaamGarNode*> (node);
    const FirtaNode* FN = dynamic_cast<const FirtaNode*> (node);


    if(KGN != nullptr){
        Value functionValue = evaluate(KGN->function.get(), env);
        if(!std::holds_alternative<FunctionValue>(functionValue))
            throw std::runtime_error("Attempted to call a non-function");
        FunctionValue function = std::get<FunctionValue>(functionValue);
        const KaamNode* KM = function.definition;
        Environment functionEnv(function.definedEnv);
        if(KGN->arguments.size() != KM->parameters.size())
            throw std::runtime_error("Parameters and arguments must match");
        for(int i = 0 ; i <  KGN->arguments.size(); i++){
            Value value =  evaluate(KGN->arguments[i].get(), env);
            functionEnv.declareVariables(KM->parameters[i], value);
        }

        try{
            for (const auto& body : KM->body_statements)
                evaluate(body.get(), functionEnv);
        }
        catch (const ReturnSignal& signal) {
            return signal.value;
        }

        return 0.0;
        
    }

    if (FN != nullptr) {
        Value result = 0.0;
        if (FN->value != nullptr)
            result = evaluate(FN->value.get(), env);
        throw ReturnSignal{result};
    }

    if(KN != nullptr){
        FunctionValue function{KN, &env};
        env.declareVariables(KN->name, function);
        return 0.0;
    }

    if(JN != nullptr){
        Value conditional = evaluate(JN->conditional_statement.get(), env);
        if(!std::holds_alternative<double>(conditional))
            throw std::runtime_error("Condition must be numeric");
        double condition = std::get<double>(conditional);
        while(condition != 0.0){
            Environment child(&env);
            for(const auto& body : JN->body_statements)
                evaluate(body.get(), child); 

            conditional = evaluate(JN->conditional_statement.get(), env);
            if(!std::holds_alternative<double>(conditional))
                throw std::runtime_error("Condition must be numeric");
            condition = std::get<double>(conditional);
        }
        return 0.0;
    }

    if(YN!=nullptr){
        Value conditional = evaluate(YN->conditional_statement.get(), env);
        if(!std::holds_alternative<double>(conditional))
            throw std::runtime_error("Condition must be numeric");

        double condition = std::get<double>(conditional);
        if(condition != 0.0){
            Environment child(&env);
            for(const auto& body : YN->body_statements)
                evaluate(body.get(), child);  
        }
        else if(YN->alt.get() != nullptr)
            evaluate(YN->alt.get(), env);
    
        return 0.0;
    }

    if(const auto* NN = dynamic_cast<const NatraNode*>(node)) {
        Environment child(&env);
        for (const auto& body : NN->body_statements)
            evaluate(body.get(), child);
        return 0.0;
    }

    if(DN != nullptr){
        const Expr* right_node = DN->initializer.get();
        Value rightValue = evaluate(right_node, env);
        env.declareVariables(DN->name, rightValue);
        return rightValue;
    }

    if(AN != nullptr){
        const Expr* right_node = AN->right.get();
        Value rightValue = evaluate(right_node, env);
        env.storeVariables(AN->name,rightValue);
        return rightValue;
    }

    if(LN != nullptr)
        return LN->value;
        
    if(IN != nullptr)
        return env.getVariables(IN->name);

    if(CN != nullptr){
        Value expr = evaluate(CN->expression.get() , env);
        if(std::holds_alternative<std::string>(expr))
            std::cout<<std::get<std::string>(expr);
        else if(std::holds_alternative<double>(expr))
            std::cout<<std::get<double>(expr);
        return 0.0;
    }

    if(BN != nullptr){
        const Expr* left_node = BN->left.get();
        const Expr* right_node = BN->right.get();
        Value leftValue = evaluate(left_node, env);
        Value rightValue = evaluate(right_node, env);

        if(std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)){
            double left = std::get<double>(leftValue);
            double right = std::get<double>(rightValue);
            if(BN -> op == categoryType::EQUAL_EQUAL)
                return left == right ? 1.0 : 0.0;
            if(BN -> op == categoryType::NOT_EQUAL) 
                return left != right ? 1.0 : 0.0;
            if(BN -> op == categoryType::SMALLER_THAN)
                return left < right ? 1.0 : 0.0;
            if(BN -> op == categoryType::SMALLER_OR_EQUAL_TO)
                return left <= right ? 1.0 : 0.0;
            if(BN -> op == categoryType::GREATER_THAN)
                return left > right ? 1.0 : 0.0;
            if(BN -> op == categoryType::GREATER_OR_EQUAL_TO)
                return left >= right ? 1.0 : 0.0;

            if(BN -> op == categoryType::PLUS)
                return left + right;
            if(BN -> op == categoryType::MINUS)
                return left - right;
            if(BN->op == categoryType::MULTIPLY)
                return left * right;
            if(BN -> op == categoryType::DIVIDE){
                if(right == 0)
                    throw std::runtime_error("Division by zero is undefined");
                return left / right;
            }
            throw  std::runtime_error("Unsupported operator");
        }
        else if(std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)){
            std::string left = std::get<std::string>(leftValue);
            std::string right = std::get<std::string>(rightValue);
            if(BN -> op == categoryType::PLUS)
                return left + right;
            if(BN -> op == categoryType::EQUAL_EQUAL) 
                return left == right ? 1.0 : 0.0;
            if(BN -> op == categoryType::NOT_EQUAL) 
                return left != right ? 1.0 : 0.0;
            throw std::runtime_error("Unsupported operator with strings");
        }
        throw std::runtime_error("Incompatible operand types");
    }
    throw std::runtime_error("Unknown or invalid AST node");
}

