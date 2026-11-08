#include "Lexer.hpp"
#include "LexerError.hpp"
#include "Parser.hpp"
#include "ParserError.hpp"
#include "Evaluator.hpp"
#include "Environment.hpp"
#include "Value.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

struct OutputCapture {
    std::ostringstream output;
    std::streambuf* old;
    OutputCapture():old(std::cout.rdbuf(output.rdbuf())){}
    ~OutputCapture(){std::cout.rdbuf(old);}
};

std::string run(const std::string& source) {
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    std::vector<std::unique_ptr<Expr>> program;
    parser.parseProgram(program);
    Environment env;
    OutputCapture capture;
    for(const auto& stmt:program)
        evaluate(stmt.get(),env);
    return capture.output.str();
}

struct Good {const char* name;std::string source;std::string expected;};
struct Bad {const char* name;std::string source;std::string error;};

int main() {
    const std::vector<Good> valid = {
        {"arithmetic precedence","chapa 2 + 3 * 4;","14"},
        {"parentheses","chapa (2 + 3) * 4;","20"},
        {"unary minus","chapa - -5;","5"},
        {"decimal arithmetic","chapa 1.5 + 2.25;","3.75"},
        {"comparisons","chapa 5 > 2; chapa 2 >= 2; chapa 3 < 1; chapa 4 != 4;","1100"},
        {"variable assignment","manam x := 10; x := x + 5; chapa x;","15"},
        {"strings and concatenation","manam first := \"Hello\"; chapa first + \" World\";","Hello World"},
        {"string comparison","chapa \"abc\" == \"abc\"; chapa \"abc\" != \"xyz\";","11"},
        {"if true","manam x := 5; yedi x > 0 { chapa \"yes\"; } natra { chapa \"no\"; }","yes"},
        {"if false","manam x := 0; yedi x > 0 { chapa \"yes\"; } natra { chapa \"no\"; }","no"},
        {"else if","manam x := 7; yedi x > 10 { chapa \"a\"; } navaye x > 5 { chapa \"b\"; } natra { chapa \"c\"; }","b"},
        {"while loop","manam x := 0; jabasamma x < 4 { chapa x; x := x + 1; }","0123"},
        {"while not entered","jabasamma 0 { chapa 99; } chapa 1;","1"},
        {"block shadowing","manam x := 10; yedi 1 { manam x := 20; chapa x; } chapa x;","2010"},
        {"write parent binding","manam x := 10; yedi 1 { x := 25; } chapa x;","25"},
        {"basic function","kaam add(a,b) { firta a + b; } chapa kaam_gar(add(10,20));","30"},
        {"function result in declaration","kaam twice(n) { firta n * 2; } manam x := kaam_gar(twice(8)); chapa x;","16"},
        {"return from if","kaam check(x) { yedi x > 10 { firta 100; } firta 0; } chapa kaam_gar(check(20));","100"},
        {"return false branch","kaam check(x) { yedi x > 10 { firta 100; } firta 0; } chapa kaam_gar(check(5));","0"},
        {"return skips later statements","kaam f() { firta 7; chapa \"bad\"; } chapa kaam_gar(f());","7"},
        {"bare return","kaam f() { firta; chapa \"bad\"; } chapa kaam_gar(f());","0"},
        {"no return defaults to zero","kaam f() { manam x := 1; } chapa kaam_gar(f());","0"},
        {"return through loop and if","kaam f() { manam i := 0; jabasamma i < 10 { yedi i == 4 { firta i; } i := i + 1; } firta 99; } chapa kaam_gar(f());","4"},
        {"factorial recursion","kaam fact(n) { yedi n <= 1 { firta 1; } firta n * kaam_gar(fact(n - 1)); } chapa kaam_gar(fact(5));","120"},
        {"recursion with locals","kaam sum(n) { yedi n <= 0 { firta 0; } manam current := n; manam previous := kaam_gar(sum(n - 1)); firta current + previous; } chapa kaam_gar(sum(5));","15"},
        {"separate call environments","kaam next(n) { manam value := n + 1; firta value; } chapa kaam_gar(next(1)); chapa kaam_gar(next(10));","211"},
        {"definition scope","manam bonus := 5; kaam add(a,b) { firta a + b + bonus; } chapa kaam_gar(add(10,20));","35"},
        {"caller evaluates arguments","manam x := 9; kaam f(x) { firta x; } chapa kaam_gar(f(x + 1));","10"},
        {"nested calls","kaam inc(x) { firta x + 1; } chapa kaam_gar(inc(kaam_gar(inc(4))));","6"},
        {"function may print","kaam greet(name) { chapa \"Hello \" + name; firta 0; } kaam_gar(greet(\"Nepal\"));","Hello Nepal"}
    };

    const std::vector<Bad> invalid = {
        {"unknown identifier","chapa missing;","Unknown identifer found"},
        {"undeclared assignment","x := 5;","Identifier not declared"},
        {"duplicate variable","manam x := 1; manam x := 2;","Identifier already declared"},
        {"division by zero","chapa 10 / 0;","Division by zero"},
        {"mixed addition","chapa 1 + \"abc\";","Incompatible operand types"},
        {"unsupported string operator","chapa \"a\" * \"b\";","Unsupported operator with strings"},
        {"string condition","yedi \"yes\" { chapa 1; }","Condition must be numeric"},
        {"calling number","manam x := 3; chapa kaam_gar(x());","Attempted to call a non-function"},
        {"too many arguments","kaam f(a) { firta a; } chapa kaam_gar(f(1,2));","Parameters and arguments must match"},
        {"too few arguments","kaam f(a,b) { firta a; } chapa kaam_gar(f(1));","Parameters and arguments must match"},
        {"unknown function","chapa kaam_gar(missing());","Unknown identifer found"},
        {"missing semicolon","manam x := 10","Expected semicolon after expression"},
        {"bad expression","manam x := 10 + * 5;","Expected expression"},
        {"invalid function syntax","kaam (x) { firta x; }","Expected function name"}
    };

    int failed=0,total=0;
    for(const auto& tc:valid) {
        ++total;
        try {
            auto actual=run(tc.source);
            if(actual!=tc.expected) {
                ++failed;
                std::cerr<<"FAIL "<<tc.name<<" expected ["<<tc.expected<<"] got ["<<actual<<"]\n";
            } else std::cout<<"PASS "<<tc.name<<"\n";
        } catch(const std::exception& e) {
            ++failed;
            std::cerr<<"FAIL "<<tc.name<<" threw "<<e.what()<<"\n";
        }
    }

    for(const auto& tc:invalid) {
        ++total;
        try {
            run(tc.source);
            ++failed;
            std::cerr<<"FAIL "<<tc.name<<" unexpectedly succeeded\n";
        } catch(const std::exception& e) {
            if(std::string(e.what()).find(tc.error)==std::string::npos) {
                ++failed;
                std::cerr<<"FAIL "<<tc.name<<" wrong error: "<<e.what()<<"\n";
            } else std::cout<<"PASS "<<tc.name<<"\n";
        }
    }
    std::cout<<"Interpreter "<<total-failed<<"/"<<total<<" passed\n";
    return failed?1:0;
}
