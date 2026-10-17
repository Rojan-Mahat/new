#include "Token.hpp"
#include "Lexer.hpp"
#include "LexerError.hpp"

#include<vector>
#include<iostream>


std::string validSource =
    "~ LEXER STRESS TEST\n"
    "make player_123 := 99.75\n"
    "make _health := 100\n"
    "make message := \"Hello \\\"World\\\"!\"\n"
    "make path := \"C:\\\\Games\\\\new\"\n"
    "make multiline := \"First line\n"
    "Second ~ not a comment\n"
    "Third line\"\n"
    "when (_health >= 50) {\n"
    "    player_123 += 10\n"
    "    _health--\n"
    "    player_123 *= 2.5\n"
    "}\n"
    "otherwise {\n"
    "    task calculate(a, b) {\n"
    "        make result := a + b / 2\n"
    "    }\n"
    "}\n"
    "repeat_until (player_123 == 0)\n"
    "maker+++_abc\n"
    "[make, when, task];\n"
    "~ END";

std::vector<Token> validExpected = {
    {"make", 2, categoryType::MAKE},
    {"player_123", 2, categoryType::IDENTIFIER},
    {":=", 2, categoryType::ASSIGNMENT},
    {"99.75", 2, categoryType::NUMBER},

    {"make", 3, categoryType::MAKE},
    {"_health", 3, categoryType::IDENTIFIER},
    {":=", 3, categoryType::ASSIGNMENT},
    {"100", 3, categoryType::NUMBER},

    {"make", 4, categoryType::MAKE},
    {"message", 4, categoryType::IDENTIFIER},
    {":=", 4, categoryType::ASSIGNMENT},
    {"Hello \\\"World\\\"!", 4, categoryType::STRING},

    {"make", 5, categoryType::MAKE},
    {"path", 5, categoryType::IDENTIFIER},
    {":=", 5, categoryType::ASSIGNMENT},
    {"C:\\\\Games\\\\new", 5, categoryType::STRING},

    {"make", 6, categoryType::MAKE},
    {"multiline", 6, categoryType::IDENTIFIER},
    {":=", 6, categoryType::ASSIGNMENT},
    {"First line\nSecond ~ not a comment\nThird line", 6, categoryType::STRING},

    {"when", 9, categoryType::WHEN},
    {"(", 9, categoryType::LEFT_PAREN},
    {"_health", 9, categoryType::IDENTIFIER},
    {">=", 9, categoryType::GREATER_OR_EQUAL_TO},
    {"50", 9, categoryType::NUMBER},
    {")", 9, categoryType::RIGHT_PAREN},
    {"{", 9, categoryType::LEFT_BRACE},

    {"player_123", 10, categoryType::IDENTIFIER},
    {"+=", 10, categoryType::ADDITION_ASSIGNMENT},
    {"10", 10, categoryType::NUMBER},

    {"_health", 11, categoryType::IDENTIFIER},
    {"--", 11, categoryType::DECREMENT},

    {"player_123", 12, categoryType::IDENTIFIER},
    {"*=", 12, categoryType::MULTIPLICATION_ASSIGNMENT},
    {"2.5", 12, categoryType::NUMBER},

    {"}", 13, categoryType::RIGHT_BRACE},

    {"otherwise", 14, categoryType::OTHERWISE},
    {"{", 14, categoryType::LEFT_BRACE},

    {"task", 15, categoryType::TASK},
    {"calculate", 15, categoryType::IDENTIFIER},
    {"(", 15, categoryType::LEFT_PAREN},
    {"a", 15, categoryType::IDENTIFIER},
    {",", 15, categoryType::COMMA},
    {"b", 15, categoryType::IDENTIFIER},
    {")", 15, categoryType::RIGHT_PAREN},
    {"{", 15, categoryType::LEFT_BRACE},

    {"make", 16, categoryType::MAKE},
    {"result", 16, categoryType::IDENTIFIER},
    {":=", 16, categoryType::ASSIGNMENT},
    {"a", 16, categoryType::IDENTIFIER},
    {"+", 16, categoryType::PLUS},
    {"b", 16, categoryType::IDENTIFIER},
    {"/", 16, categoryType::DIVIDE},
    {"2", 16, categoryType::NUMBER},

    {"}", 17, categoryType::RIGHT_BRACE},
    {"}", 18, categoryType::RIGHT_BRACE},

    {"repeat_until", 19, categoryType::REPEAT_UNTIL},
    {"(", 19, categoryType::LEFT_PAREN},
    {"player_123", 19, categoryType::IDENTIFIER},
    {"==", 19, categoryType::EQUAL_EQUAL},
    {"0", 19, categoryType::NUMBER},
    {")", 19, categoryType::RIGHT_PAREN},

    {"maker", 20, categoryType::IDENTIFIER},
    {"++", 20, categoryType::INCREMENT},
    {"+", 20, categoryType::PLUS},
    {"_abc", 20, categoryType::IDENTIFIER},

    {"[", 21, categoryType::LEFT_SQUARE},
    {"make", 21, categoryType::MAKE},
    {",", 21, categoryType::COMMA},
    {"when", 21, categoryType::WHEN},
    {",", 21, categoryType::COMMA},
    {"task", 21, categoryType::TASK},
    {"]", 21, categoryType::RIGHT_SQUARE},
    {";", 21, categoryType::SEMICOLON}
};


std::string invalidSource =
    "make health := 100\n"
    "make message := \"Safe ~ text\"\n"
    "~ An ignored comment\n"
    "when (health >= 50) {\n"
    "    make value := 12.5\n"
    "    health += 10\n"
    "    make broken := 123_abc\n"
    "    make neverReached := 500\n"
    "}\n";
int expectedLine = 7;


bool tester(const std::string& source , const std::vector<Token>& expected){
    Lexer lexer(source);
    std::vector<Token> actual = lexer.tokenize();
    return (actual == expected);
}

bool errorTester(const std::string& source,int expectedLine){
    Lexer lexer(source);
    try{
        std::vector<Token> actual = lexer.tokenize();
        return false;
    }
    catch(const LexerError& e){
        return e.getLine() == expectedLine;
    }
}

int main(){ 
    bool validPassed = tester(validSource, validExpected);
    bool errorPassed = errorTester(invalidSource, 7);
    bool allPassed = validPassed && errorPassed;
    std::cout << (allPassed ? "ALL TESTS PASSED\n"
                           : "SOME TESTS FAILED\n");
    return allPassed ? 0 : 1;
}