#include "Lexer.hpp"
#include "Parser.hpp"
#include "ParserError.hpp"

#include<iostream>
#include <sstream>

const BinaryNode* asBinary(const Expr* root) {
    return dynamic_cast<const BinaryNode*>(root);
}

const NumberNode* asNumber(const Expr* root){
    return dynamic_cast<const NumberNode*>(root);
}

std::string formatNumber(double value){
    std::ostringstream stream;
    stream << value;           
    std::string result = stream.str(); 
    return result;
}

void traverse(const Expr* node, std::vector<std::string>& testResult){
    const NumberNode* NN = asNumber(node);
    const BinaryNode* BN = asBinary(node);

    if(NN != nullptr){
        testResult.push_back(formatNumber(NN->value));
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

std::vector<bool> tester(const std::vector<std::string>& sources , const  std::vector<std::vector<std::string>> & expectations){
    if(sources.size()!=expectations.size())
        return {};
    
    std::vector<bool> returningResults;
    for(size_t i = 0 ; i < sources.size(); i++){
        Lexer lexer(sources[i]);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto root = parser.parse(); 

        std::vector<std::string> testResult;
        traverse(root.get(), testResult);
        
        returningResults.push_back((testResult == expectations[i]) ? 1 : 0);
    }
    return returningResults;
}

std::vector<bool> errorTestor(const std::vector<std::string>& invalidSources, 
    const std::vector<std::string>& errorExpectations,
    const std::vector<int>& expectedLines){
    if(invalidSources.size() != errorExpectations.size())
        return {};
    if(invalidSources.size() != expectedLines.size())
        return {};
    std::vector<bool> returningResults;
    for(size_t i = 0 ; i < invalidSources.size(); i++){
        try{
            Lexer lexer(invalidSources[i]);
            auto tokens = lexer.tokenize();
            Parser parser(tokens);
            auto root = parser.parse(); 
            returningResults.push_back(false);
        }

        catch(const ParserError& e){
            std::string message = e.what();
            int line = e.getLine();
            bool passed = message.find(errorExpectations[i]) != std::string::npos && line == expectedLines[i];
            returningResults.push_back(passed);
        }

        catch (const std::exception& e){
            returningResults.push_back(false);
        }
    }
    return returningResults;
}

int getValidResult( std::vector<std::string> sources, std::vector<std::vector<std::string>> expectations){
    bool failed = false;
    std::vector<bool> finalResults = tester(sources, expectations);
    if(finalResults.empty()){
        std::cout<<"Sources and Expectations do not match! ";
        return 1;
    }
    for(size_t i = 0 ; i < finalResults.size(); i++){
        if(!finalResults[i]){
            std::cout<<"Failed the tests at index "<<i << ". Failed Statement below :- \n";
            std::cout<<sources[i]<<" \n \n";
            failed = true;
        }    
    }
    if(!failed){
        std::cout<<"ALL VALID TESTS PASSED! ";
        return 0;
    }
    std::cout<<"SOME VALID TESTS FAILED ";
    return 1;
}

int getErrorResult(std::vector<std::string> invalidSources , std::vector<std::string> errorExpectations, std::vector<int> expectedLines){
    bool failed = false;
    std::vector<bool> finalResults = errorTestor(invalidSources, errorExpectations , expectedLines);
    if(finalResults.empty()){
        std::cout<<"Sources and Expectations do not match! ";
        return 1;
    }
    for(size_t i = 0 ; i < finalResults.size(); i++){
        if(!finalResults[i]){
            std::cout<<"Failed the tests at index "<<i << ". Failed Statement below :- \n";
            std::cout<<invalidSources[i]<<" \n \n";
            failed = true;
        }    
    }
    if(!failed){
        std::cout<<"ALL ERROR TESTS PASSED! ";
        return 0;
    }
    std::cout<<"SOME ERROR TESTS FAILED ";
    return 1;
}


int main(){
    std::vector<std::string> sources = {
        "3.75 + 2.5 * 4",
        "0.125 * 8.5",
        "(12.5 - 3.25) / 2.5",
        "1.0 + 2.00 * 3.500",
        "((0.5 + 1.25) * (4.75 - 2.5)) / 3.0"
    };
    std::vector<std::vector<std::string>> expectations = {
        {"PLUS", "3.75", "MULTIPLY", "2.5", "4"},
        {"MULTIPLY", "0.125", "8.5"},
        {"DIVIDE", "MINUS", "12.5", "3.25", "2.5"},
        {"PLUS", "1", "MULTIPLY", "2", "3.5"},
        {"DIVIDE", "MULTIPLY", "PLUS", "0.5", "1.25",
        "MINUS", "4.75", "2.5", "3"}
    };

    std::vector<std::string> invalidSources = {
        "2 + * 3",
        "3+",
        "(2 + 3",
        "(2--3)",
        "2 ++ 3",
        "2 3",
        "((2 + 3) * 4",
        "2 + (3 * / 4)",
        "()",
        ""
    };
    std::vector<std::string> errorExpectations = {
        "Expected expression, found 'MULTIPLY'",
        "Expected expression, reached end of input",
        "Parenthesis not closed",
        "Unexpected Token'DECREMENT'",
        "Unexpected token 'INCREMENT' after expression",
        "Unexpected token 'NUMBER' after expression",
        "Parenthesis not closed",
        "Expected expression, found 'DIVIDE'",
        "Expected expression, found 'RIGHT_PAREN'",
        "Expected expression, reached end of input"
    };
    std::vector<int> expectedLines = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };

    int validStatus = getValidResult(sources, expectations);
    std::cout << '\n';

    int errorStatus = getErrorResult(invalidSources,errorExpectations,expectedLines);
    std::cout << '\n';

    return (validStatus == 0 && errorStatus == 0) ? 0 : 1;
}


