#include "Lexer.hpp"
#include "LexerError.hpp"
#include <iostream>
#include <vector>
#include <string>

struct Case { const char* name; std::string source; std::vector<Token> tokens; };
int main() {
    const std::vector<Case> valid = {
        {"keywords", "manam yedi navaye natra jabasamma kaam kaam_gar firta chapa",
        {{"manam",1,MANAM},{"yedi",1,YEDI},{"navaye",1,NAVAYE},{"natra",1,NATRA},{"jabasamma",1,JABASAMMA},{"kaam",1,KAAM},{"kaam_gar",1,KAAMGAR},{"firta",1,FIRTA},{"chapa",1,CHAPA}}},
        {"identifiers", "_name12 abc kaam_extra firta2", {{"_name12",1,IDENTIFIER},{"abc",1,IDENTIFIER},{"kaam_extra",1,IDENTIFIER},{"firta2",1,IDENTIFIER}}},
        {"numbers", "0 15 3.75", {{"0",1,NUMBER},{"15",1,NUMBER},{"3.75",1,NUMBER}}},
        {"operators", ":= + - * / == != < <= > >= ++ -- += -= *= /=",
        {{":=",1,ASSIGNMENT},{"+",1,PLUS},{"-",1,MINUS},{"*",1,MULTIPLY},{"/",1,DIVIDE},{"==",1,EQUAL_EQUAL},{"!=",1,NOT_EQUAL},{"<",1,SMALLER_THAN},{"<=",1,SMALLER_OR_EQUAL_TO},{">",1,GREATER_THAN},{">=",1,GREATER_OR_EQUAL_TO},{"++",1,INCREMENT},{"--",1,DECREMENT},{"+=",1,ADDITION_ASSIGNMENT},{"-=",1,SUBTRACTION_ASSIGNMENT},{"*=",1,MULTIPLICATION_ASSIGNMENT},{"/=",1,DIVISION_ASSIGNMENT}}},
        {"punctuation", "(){},;[]", {{"(",1,LEFT_PAREN},{")",1,RIGHT_PAREN},{"{",1,LEFT_BRACE},{"}",1,RIGHT_BRACE},{",",1,COMMA},{";",1,SEMICOLON},{"[",1,LEFT_SQUARE},{"]",1,RIGHT_SQUARE}}},
        {"comments and line numbers", "manam x := 1; ~ ignored\nchapa x;\nfirta;", {{"manam",1,MANAM},{"x",1,IDENTIFIER},{":=",1,ASSIGNMENT},{"1",1,NUMBER},{";",1,SEMICOLON},{"chapa",2,CHAPA},{"x",2,IDENTIFIER},{";",2,SEMICOLON},{"firta",3,FIRTA},{";",3,SEMICOLON}}},
        {"string escapes", R"src("a\n\"b\"\\c" "a\tb")src", {{std::string("a\n\"b\"\\c"),1,STRING},{"a    b",1,STRING}}},
        {"multiline string", "\"first\nsecond\" chapa", {{"first\nsecond",1,STRING},{"chapa",2,CHAPA}}},
        {"empty input", "", {}},
        {"comment only", "~ no tokens", {}}
    };
    struct Bad {
        const char* name; 
        std::string source; 
        int line; 
    };
    const std::vector<Bad> invalid = {
        {"letters after number","123abc",1},
        {"two decimal points","1.2.3",1},
        {"leading decimal",".5",1},
        {"trailing decimal","5.",1},
        {"unknown symbol","manam x := 1;\n@",2},
        {"invalid operator",":",1},
        {"unterminated string","\"hello",1},
        {"unknown escape",R"src("a\q")src",1},
        {"unfinished escape",std::string("\"hi") + '\\',1}
    };
    int failed=0,total=0;
    for(const auto& tc:valid){
        ++total;
        try{
            Lexer lexer(tc.source);
            auto got=lexer.tokenize();
            if(got!=tc.tokens) {
                ++failed;
                std::cerr<<"FAIL "<<tc.name<<" expected "<<tc.tokens.size()<<" tokens, got "<<got.size()<<"\n";
                for(size_t i=0;i<got.size();++i)
                    if(i>=tc.tokens.size() || !(got[i]==tc.tokens[i]))
                        std::cerr<<"  mismatched token "<<i<<": "<<got[i].original<<" line "<<got[i].line<<"\n";
            }
            else std::cout<<"PASS "<<tc.name<<"\n";
        }

        catch(const std::exception& e){
            ++failed;
            std::cerr<<"FAIL "<<tc.name<<": "<<e.what()<<"\n";
        }
    }

    for(const auto& tc:invalid) {
        ++total;
        try { Lexer lexer(tc.source); lexer.tokenize(); ++failed; std::cerr<<"FAIL "<<tc.name<<" did not throw\n"; }
        catch(const LexerError& e) {
            if(e.getLine()!=tc.line){
                ++failed;
                std::cerr<<"FAIL "<<tc.name<<" wrong line "<<e.getLine()<<"\n";
            }
            else
                std::cout<<"PASS "<<tc.name<<"\n";
        }
        catch(const std::exception& e){
            ++failed;
            std::cerr<<"FAIL "<<tc.name<<" wrong error: "<<e.what()<<"\n";
        }
    }
    std::cout<<"Lexer "<<total-failed<<"/"<<total<<" passed\n";
    return failed?1:0;
}
