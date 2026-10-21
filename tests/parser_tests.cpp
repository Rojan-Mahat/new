#include "Lexer.hpp"
#include "Parser.hpp"

#include<iostream>
BinaryNode* asBinary(Expr* root){
    return dynamic_cast<BinaryNode*>(root);
}

const BinaryNode* asBinary(const Expr* root) {
    return dynamic_cast<const BinaryNode*>(root);
}

NumberNode* asNumber(Expr* root){
    return dynamic_cast<NumberNode*>(root);
}

const NumberNode* asNumber(const Expr* root){
    return dynamic_cast<const NumberNode*>(root);
}

void traverse(const Expr* node, std::vector<std::string>& testResult){
    const NumberNode* NN = asNumber(node);
    const BinaryNode* BN = asBinary(node);

    if(NN != nullptr){
        testResult.push_back(std::to_string(static_cast<int> (NN->value)));
        return;
    }

    if(BN != nullptr){
        testResult.push_back(tokenTypeToString(BN->op));
        const Expr* left_node = BN->left.get();
        const Expr* right_node = BN->right.get();
        traverse(left_node, testResult);
        traverse(right_node, testResult);
    }
}

bool tester(const std::string& source , const std::vector<std::string>& expected){
    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto root = parser.parse(); 

    std::vector<std::string> testResult;

    traverse(root.get(), testResult);
    return expected == testResult;
    
}

int main(){
    std::string source = "((12 - 3 * (2 + 4)) / (7 - 5) + 8 * (6 - 2)) / 2 - 9";
    std::vector<std::string> expected = {
    "MINUS",
    "DIVIDE",
    "PLUS",
    "DIVIDE",
    "MINUS",
    "12",
    "MULTIPLY",
    "3",
    "PLUS",
    "2",
    "4",
    "MINUS",
    "7",
    "5",
    "MULTIPLY",
    "8",
    "MINUS",
    "6",
    "2",
    "2",
    "9"
};
    std::cout << (tester(source,expected) ? "Passed" : "Failed");

}


