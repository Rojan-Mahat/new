#include "Token.hpp"
#include "Lexer.hpp"
#include "LexerError.hpp"

#include<vector>
#include<iostream>

std::string validSource =
    "~ LEXER STRESS TEST\n"
    "manam player_123 := 99.75\n"
    "manam _health := 100\n"
    "manam message := \"Hello \\\"World\\\"!\"\n"
    "manam path := \"C:\\\\Games\\\\new\"\n"
    "manam multiline := \"First line\n"
    "Second ~ not a comment\n"
    "Third line\"\n"
    "yedi (_health >= 50) {\n"
    "    player_123 += 10\n"
    "    _health--\n"
    "    player_123 *= 2.5\n"
    "}\n"
    "natra {\n"
    "    task calculate(a, b) {\n"
    "        manam result := a + b / 2\n"
    "    }\n"
    "}\n"
    "repeat_until (player_123 == 0)\n"
    "maker+++_abc\n"
    "[manam, yedi, natra, task];\n"
    "~ END";


std::vector<Token> validExpected = {
    {"manam", 2, categoryType::MANAM},
    {"player_123", 2, categoryType::IDENTIFIER},
    {":=", 2, categoryType::ASSIGNMENT},
    {"99.75", 2, categoryType::NUMBER},

    {"manam", 3, categoryType::MANAM},
    {"_health", 3, categoryType::IDENTIFIER},
    {":=", 3, categoryType::ASSIGNMENT},
    {"100", 3, categoryType::NUMBER},

    {"manam", 4, categoryType::MANAM},
    {"message", 4, categoryType::IDENTIFIER},
    {":=", 4, categoryType::ASSIGNMENT},
    {"Hello \\\"World\\\"!", 4, categoryType::STRING},

    {"manam", 5, categoryType::MANAM},
    {"path", 5, categoryType::IDENTIFIER},
    {":=", 5, categoryType::ASSIGNMENT},
    {"C:\\\\Games\\\\new", 5, categoryType::STRING},

    {"manam", 6, categoryType::MANAM},
    {"multiline", 6, categoryType::IDENTIFIER},
    {":=", 6, categoryType::ASSIGNMENT},
    {"First line\nSecond ~ not a comment\nThird line", 6, categoryType::STRING},

    {"yedi", 9, categoryType::YEDI},
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

    {"natra", 14, categoryType::NATRA},
    {"{", 14, categoryType::LEFT_BRACE},

    {"task", 15, categoryType::TASK},
    {"calculate", 15, categoryType::IDENTIFIER},
    {"(", 15, categoryType::LEFT_PAREN},
    {"a", 15, categoryType::IDENTIFIER},
    {",", 15, categoryType::COMMA},
    {"b", 15, categoryType::IDENTIFIER},
    {")", 15, categoryType::RIGHT_PAREN},
    {"{", 15, categoryType::LEFT_BRACE},

    {"manam", 16, categoryType::MANAM},
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
    {"manam", 21, categoryType::MANAM},
    {",", 21, categoryType::COMMA},
    {"yedi", 21, categoryType::YEDI},
    {",", 21, categoryType::COMMA},
    {"natra", 21, categoryType::NATRA},
    {",", 21, categoryType::COMMA},
    {"task", 21, categoryType::TASK},
    {"]", 21, categoryType::RIGHT_SQUARE},
    {";", 21, categoryType::SEMICOLON}
};

std::string invalidSource =
    "manam health := 100\n"
    "manam message := \"Safe ~ text\"\n"
    "~ An ignored comment\n"
    "yedi (health >= 50) {\n"
    "    manam value := 12.5\n"
    "    health += 10\n"
    "    manam broken := 123_abc\n"
    "    manam neverReached := 500\n"
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