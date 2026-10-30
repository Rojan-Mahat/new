#include "Evaluator.hpp"
#include "Environment.hpp"

double evaluate(const Expr* node, Environment& env){
    const NumberNode* NN = dynamic_cast <const NumberNode*> (node);
    const BinaryNode* BN = dynamic_cast<const BinaryNode*> (node);
    const IdentifierNode* IN = dynamic_cast<const IdentifierNode*> (node);
    const AssignmentNode* AN = dynamic_cast<const AssignmentNode*> (node);
    const DeclarationNode* DN = dynamic_cast<const DeclarationNode*> (node);

    if(DN != nullptr){
        const Expr* right_node = DN->initializer.get();
        double rightValue = evaluate(right_node, env);
        env.declareVariables(DN->name, rightValue);
        return rightValue;
    }

    if(AN != nullptr){
        const Expr* right_node = AN->right.get();
        double rightValue = evaluate(right_node, env);
        env.storeVariables(AN->name,rightValue);
        return rightValue;
    }

    if(NN != nullptr)
        return NN->value;
    if(IN != nullptr)
        return env.getVariables(IN->name);
    if(BN != nullptr){
        const Expr* left_node = BN->left.get();
        const Expr* right_node = BN->right.get();
        double leftValue = evaluate(left_node, env);
        double rightValue = evaluate(right_node, env);

        if(BN -> op == categoryType::PLUS)
            return leftValue + rightValue;
        if(BN -> op == categoryType::MINUS)
            return leftValue - rightValue;
        if(BN->op == categoryType::MULTIPLY)
            return leftValue * rightValue;
        if(BN -> op == categoryType::DIVIDE){
            if(rightValue == 0)
                throw std::runtime_error("Division by zero is undefined");
            return leftValue / rightValue;
        }
        throw  std::runtime_error("Unsupported operator");
    }
    throw  std::runtime_error("Unknown or invalid AST node");
}

