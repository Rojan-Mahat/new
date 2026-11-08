#include "Lexer.hpp"
#include "Parser.hpp"
#include "ParserError.hpp"
#include "AST.hpp"
#include <iostream>
#include <functional>
#include <vector>
#include <string>

using Program = std::vector<std::unique_ptr<Expr>>;

Program parse(const std::string& source) {
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    Program program;
    parser.parseProgram(program);
    return program;
}
template<class T> const T* as(const Expr* node) {
    return dynamic_cast<const T*>(node);
}
bool number(const Expr* expr,double expected) {
    auto p=as<LiteralNode>(expr);
    return p && std::holds_alternative<double>(p->value)
        && std::get<double>(p->value)==expected;
}
bool identifier(const Expr* expr,const std::string& name) {
    auto p=as<IdentifierNode>(expr);
    return p && p->name==name;
}
bool binary(const Expr* expr,categoryType op) {
    auto p=as<BinaryNode>(expr);
    return p && p->op==op;
}
struct Case {
    const char* name;
    std::string source;
    std::function<bool(const Program&)> check;
};
int main() {
    const std::vector<Case> valid = {
        {"precedence","1 + 2 * 3;",[](const Program& p){
            if(p.size()!=1) return false;
            auto plus=as<BinaryNode>(p[0].get());
            return plus && plus->op==PLUS && number(plus->left.get(),1)
                && binary(plus->right.get(),MULTIPLY);
        }},
        {"parentheses","(1 + 2) * 3;",[](const Program& p){
            auto mul=p.size()==1?as<BinaryNode>(p[0].get()):nullptr;
            return mul && mul->op==MULTIPLY && binary(mul->left.get(),PLUS) && number(mul->right.get(),3);
        }},
        {"unary minus","-5;",[](const Program& p){
            auto minus=p.size()==1?as<BinaryNode>(p[0].get()):nullptr;
            return minus && minus->op==MINUS && number(minus->left.get(),0) && number(minus->right.get(),5);
        }},
        {"declaration","manam total := 2 + 3;",[](const Program& p){
            auto d=p.size()==1?as<DeclarationNode>(p[0].get()):nullptr;
            return d && d->name=="total" && binary(d->initializer.get(),PLUS);
        }},
        {"assignment","total := 9;",[](const Program& p){
            auto a=p.size()==1?as<AssignmentNode>(p[0].get()):nullptr;
            return a && a->name=="total" && number(a->right.get(),9);
        }},
        {"conditional branches","yedi x > 0 { chapa x; } navaye x == 0 { chapa 0; } natra { chapa -1; }",[](const Program& p){
            auto y=p.size()==1?as<YediNode>(p[0].get()):nullptr;
            auto other=y?as<YediNode>(y->alt.get()):nullptr;
            return y && binary(y->conditional_statement.get(),GREATER_THAN)
                && y->body_statements.size()==1 && as<ChapaNode>(y->body_statements[0].get())
                && other && binary(other->conditional_statement.get(),EQUAL_EQUAL)
                && as<NatraNode>(other->alt.get());
        }},
        {"while","jabasamma x < 5 { x := x + 1; }",[](const Program& p){
            auto w=p.size()==1?as<JabasammaNode>(p[0].get()):nullptr;
            return w && binary(w->conditional_statement.get(),SMALLER_THAN)
                && w->body_statements.size()==1 && as<AssignmentNode>(w->body_statements[0].get());
        }},
        {"function and return","kaam add(a,b) { firta a + b; }",[](const Program& p){
            auto k=p.size()==1?as<KaamNode>(p[0].get()):nullptr;
            if(!k || k->name!="add" || k->parameters!=std::vector<std::string>{"a","b"} || k->body_statements.size()!=1) return false;
            auto r=as<FirtaNode>(k->body_statements[0].get());
            return r && binary(r->value.get(),PLUS);
        }},
        {"bare return","firta;",[](const Program& p){
            auto r=p.size()==1?as<FirtaNode>(p[0].get()):nullptr;
            return r && !r->value;
        }},
        {"function call arguments","kaam_gar(add(10, 20 + x));",[](const Program& p){
            auto call=p.size()==1?as<KaamGarNode>(p[0].get()):nullptr;
            return call && identifier(call->function.get(),"add") && call->arguments.size()==2
                && number(call->arguments[0].get(),10) && binary(call->arguments[1].get(),PLUS);
        }},
        {"nested call","kaam_gar(add(kaam_gar(one()), 2));",[](const Program& p){
            auto outer=p.size()==1?as<KaamGarNode>(p[0].get()):nullptr;
            return outer && outer->arguments.size()==2 && as<KaamGarNode>(outer->arguments[0].get());
        }},
        {"empty function","kaam noop() { }",[](const Program& p){
            auto k=p.size()==1?as<KaamNode>(p[0].get()):nullptr;
            return k && k->parameters.empty() && k->body_statements.empty();
        }},
        {"multiple statements","manam x := 2; x := 3; chapa x;",[](const Program& p){
            return p.size()==3 && as<DeclarationNode>(p[0].get())
                && as<AssignmentNode>(p[1].get()) && as<ChapaNode>(p[2].get());
        }}
    };

    struct Bad {const char* name; std::string source;};
    const std::vector<Bad> invalid = {
        {"missing semicolon","manam x := 2"},
        {"missing expression","manam x := ;"},
        {"bad declaration name","manam 5 := 2;"},
        {"unclosed parenthesis","(1+2;"},
        {"missing function name","kaam (a) { firta a; }"},
        {"missing function brace","kaam add(a) firta a;"},
        {"unclosed function body","kaam add(a) { firta a;"},
        {"unclosed while","jabasamma x > 0 { x := 0;"},
        {"unclosed conditional","yedi x { chapa x;"},
        {"invalid call comma","kaam_gar(f(1 2));"},
        {"trailing call comma","kaam_gar(f(1,));"},
        {"missing call closing parenthesis","kaam_gar(f(1);"},
        {"incomplete return","firta 1 + ;"}
    };
    int failed=0,total=0;
    for(const auto& tc:valid) {
        ++total;
        try {
            auto program=parse(tc.source);
            if(!tc.check(program)) {++failed;std::cerr<<"FAIL "<<tc.name<<" AST mismatch\n";}
            else std::cout<<"PASS "<<tc.name<<"\n";
        } catch(const std::exception& e) {++failed;std::cerr<<"FAIL "<<tc.name<<": "<<e.what()<<"\n";}
    }
    for(const auto& tc:invalid) {
        ++total;
        try {
            auto program=parse(tc.source);
            ++failed;
            std::cerr<<"FAIL "<<tc.name<<" unexpectedly parsed\n";
        } catch(const ParserError&) {std::cout<<"PASS "<<tc.name<<"\n";}
        catch(const std::exception& e) {++failed;std::cerr<<"FAIL "<<tc.name<<" wrong exception: "<<e.what()<<"\n";}
    }
    std::cout<<"Parser "<<total-failed<<"/"<<total<<" passed\n";
    return failed?1:0;
}
