#include"Token.hpp"
#include<string>
std::string tokenTypeToString(categoryType type){
    switch(type){
        case categoryType::NUMBER:
            return "NUMBER";
        case categoryType::IDENTIFIER:
            return "IDENTIFIER";
        case categoryType::PLUS:
            return "PLUS";
        case categoryType::INCREMENT:
            return "INCREMENT";
        case categoryType::DECREMENT:
            return "DECREMENT";
        case categoryType::MINUS:
            return "MINUS";
        case categoryType::MULTIPLY:
            return "MULTIPLY";
        case categoryType::DIVIDE:
            return "DIVIDE";
        case categoryType::ASSIGNMENT:
            return "ASSIGNMENT";
        case categoryType::EQUAL_EQUAL:
            return "EQUAL_EQUAL";
        case categoryType::GREATER_THAN:
            return "GREATER_THAN";
        case categoryType::GREATER_OR_EQUAL_TO:
            return "GREATER_OR_EQUAL_TO";
        case categoryType::SMALLER_THAN:
            return "SMALLER_THAN";
        case categoryType::SMALLER_OR_EQUAL_TO:
            return "SMALLER_OR_EQUAL_TO";
        case categoryType::ADDITION_ASSIGNMENT:
            return "ADDITION_ASSIGNMENT";
        case categoryType::SUBTRACTION_ASSIGNMENT:
            return "SUBTRACTION_ASSIGNMENT";
        case categoryType::MULTIPLICATION_ASSIGNMENT:
            return "MULTIPLICATION_ASSIGNMENT";
        case categoryType::DIVISION_ASSIGNMENT:
            return "DIVISION_ASSIGNMENT";
    }
}
