//
//  ExprEvaluator.cpp
//  expr_evaluator

#include "expr-evaluator/ExprEvaluator.hpp"
#include <glog/logging.h>

typedef std::vector<Token> Expr_Token;

bool ExprEvaluator::is_operator(T_Type t){
    switch(t){
        case T_Type::U_MINUS:
            return true;
        case T_Type::MULTIPLY:
            return true;
        case T_Type::MINUS:
            return true;
        case T_Type::PLUS:
            return true;
        case T_Type::DIVIDE:
            return true;
        default:
            return false;
    }
}

int ExprEvaluator::operator_weight(T_Type t){
    switch(t){
        case T_Type::U_MINUS:
            return 4;
        case T_Type::MULTIPLY:
            return 3;
        case T_Type::MINUS:
            return 2;
        case T_Type::PLUS:
            return 2;
        case T_Type::DIVIDE:
            return 3;
        default:
            return -1;
    }
}

void ExprEvaluator::fix_negation(Expr_Token &exp){
    
    //Fixing two cases: if first token is eg: -1
    //                  if the previous one is left parenthesis or operator
    
    if (exp[0].type == T_Type::MINUS) {
        exp[0].type = T_Type::U_MINUS;
    }
    for(int i = 1; i < exp.size(); ++i) {
        T_Type prev = exp[i - 1].type;
        if (exp[i].type == T_Type::MINUS) {
            if (prev == T_Type::LEFT_PARENTHESIS || is_operator(prev)) {
                exp[i].type = T_Type::U_MINUS;
            }
        }
    }
}

bool ExprEvaluator::is_space(char c){
    return c == ' '  ||
    c == '\n' ||
    c == '\r' ||
    c == '\t';
}

bool ExprEvaluator::is_digit(char c){
    return '0' <= c && c <= '9';
}

bool ExprEvaluator::is_variable(char c){
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

template<class T>
T ExprEvaluator::pop(std::vector<T> &stack){
    T pop_val = stack.back();
    stack.pop_back();
    return pop_val;
}

Tokenizer_output ExprEvaluator::tokenizer(const std::string &s){
    Expr_Token exp;
    
    for(int i = 0; i < s.size(); i++) {
        if (is_space(s[i])) {
            i++;
            while (i < s.size() && is_space(s[i])) {
                i++;
            }
            if (i < s.size()) {
                i--;
            }
        } else if (is_digit(s[i])) {
            std::string number = std::string(1, s[i]);
            i++;
            
            while (i < s.size() && is_digit(s[i])) {
                number += std::string(1, s[i]);
                i++;
            }
            
            if (i < s.size()) i--;
            exp.push_back(Token{T_Type::NUMBER, stoi(number)});
        }
        
        else if (s[i] == '(') {
            exp.push_back(Token{T_Type::LEFT_PARENTHESIS, 0});
        } else if (s[i] == ')') {
            exp.push_back(Token{T_Type::RIGHT_PARENTHESIS, 0});
        } else if (s[i] == '+') {
            exp.push_back(Token{T_Type::PLUS, 0});
        } else if (s[i] == '-') {
            exp.push_back(Token{T_Type::MINUS, 0});
        } else if (s[i] == '*') {
            exp.push_back(Token{T_Type::MULTIPLY, 0});
        } else if (s[i] == '/') {
            exp.push_back(Token{T_Type::DIVIDE, 0});
        } else {
            std::string msg = "Tokenizer: unknown character '" + std::string(1, s[i]) + "'";
            return Tokenizer_output{Expr_Token{}, msg};
        }
    }
    return Tokenizer_output{exp, ""};
}

bool ExprEvaluator::replace_str(std::string &str, const std::string &from, const std::string &to){
    
    //Change later if more than one instance must be replaced
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos){
        return false;
    }
    
    str.replace (start_pos, from.length(), to);
    return true;
}

std::string ExprEvaluator::fill_variables(const std::string &expr, int level){
    std::string one_var_exp = expr;
    
    std::string str_level;
    std::ostringstream sstream;
    sstream << level;
    str_level = sstream.str();
    
    replace_str(one_var_exp, "l", str_level);
    
    return one_var_exp;
}

Calculation_output ExprEvaluator::evaluate_postfix(const Expr_Token &exp){
    std::vector<int> stack;
    bool ok = true;
    
    
    for (Token tok : exp) {
        if (tok.type == T_Type::NUMBER) {
            stack.push_back(tok.val);
        } else if (tok.type == T_Type::U_MINUS) {
            
            if (stack.size() < 1) {
                ok = false;
                break;
            }
            int a = pop(stack);
            stack.push_back(-a); //The number on top of stack must be negated
        } else {
            
            //Binary operations must have two inputs
            if (stack.size() < 2) {
                ok = false;
                break;
            }
            int a = pop(stack);
            int b = pop(stack);
            switch (tok.type) {
                case T_Type::PLUS:
                    stack.push_back(a + b);
                    break;
                case T_Type::MINUS:
                    stack.push_back(b - a);
                    break;
                case T_Type::MULTIPLY:
                    stack.push_back(a * b);
                    break;
                case T_Type::DIVIDE:
                    if (a == 0.0) {
                        return Calculation_output{0, "Division Err: Dividing by 0"};
                    }
                    stack.push_back(b / a);
                    break;
                default:
                    //Program desined to handle basic operations as seen in
                    //spells.yaml file
                    std::cout << "Evaluation Err: unknown token type '"
                    + std::string(1, char(tok.type)) + "\n";
                    break;
                    
                    
            }
        }
    }
    if (ok && stack.size() > 0) {
        return Calculation_output{stack[0], ""};
    } else {
        return Calculation_output{0, "Err: Stack is empty"};
    }
}

Calculation_output ExprEvaluator::evaluate_postfix(const std::string &expr){
    Tokenizer_output tok = tokenizer(expr);
    if(tok.is_okay()) {
        return evaluate_postfix(tok.value);
    }else {
        return Calculation_output{-1, tok.error};
    }
}


void ExprEvaluator::debug_postfix_eval(){
    for(;;){
        std::cout << "Input Eqn : ";
        std::string inp;
        std::getline(std::cin,inp);
        std::cout << "\nThe input was: " << inp << "\n";
        Calculation_output res = evaluate_postfix(inp);
        if (res.is_okay()) {
            std::cout << "The result is : " << res.value << "\n";
            
        } else {
            std::cout << "Error encountered: " << res.error << "\n";
        }
        
    }
}

Tokenizer_output ExprEvaluator::infix_to_postfix(const Expr_Token &expr){
    
    Expr_Token out;
    Expr_Token stack;
    
    for(const Token& tok : expr) {
        switch (tok.type) {
                
            case T_Type::NUMBER:
                out.push_back(tok);
                break;
                
            case T_Type::PLUS:
            case T_Type::U_MINUS:
            case T_Type::MINUS:
            case T_Type::MULTIPLY:
            case T_Type::DIVIDE:
                
                while (!stack.empty()
                       && (is_operator(stack.back().type)
                           && operator_weight(stack.back().type) >= operator_weight(tok.type)))
                {
                    out.push_back(pop(stack));
                }
                stack.push_back(tok);
                break;
                
            case T_Type::LEFT_PARENTHESIS:
                stack.push_back(tok);
                break;
                
            case T_Type::RIGHT_PARENTHESIS:
                
                while (!stack.empty()
                       && (stack.back().type != T_Type::LEFT_PARENTHESIS))
                {
                    out.push_back(pop(stack));
                }
                if (stack.empty()) {
                    return Tokenizer_output{Expr_Token{}, "Err: Check expr parenthesis"};
                } else if (stack.back().type == T_Type::LEFT_PARENTHESIS) {
                    pop(stack);
                } else {
                    //Since Spells yml files have correct equation types
                    //This else wont be reached
                    std::cout << "infix_to_postfix error: "
                    << "Check eqn parenthesis \n";
                }
                break;
            default:
                std::cout << "tok.type: '" << char(tok.type) << "'\n"
                << "Error Encountered \n";
        }
    }
    
    while (!stack.empty()) {
        Token t = pop(stack);
        if (t.type == T_Type::LEFT_PARENTHESIS || t.type == T_Type::RIGHT_PARENTHESIS) {
            return Tokenizer_output{Expr_Token{}, "Err: Check expr parenthesis"};
        }
        out.push_back(t);
    }
    
    return Tokenizer_output{out, ""};
}

Calculation_output ExprEvaluator::evaluate_infix(Expr_Token expr){
    
    fix_negation(expr);
    Tokenizer_output to_postfix = infix_to_postfix(expr);
    
    if (to_postfix.is_okay()) {
        return evaluate_postfix(to_postfix.value);
    } else {
        return Calculation_output{-1, to_postfix.error};
    }
    
}

Calculation_output ExprEvaluator::evaluate_infix(const std::string &expr){
    
    Tokenizer_output tokens = tokenizer(expr);
    if (tokens.is_okay()){
        return evaluate_infix(tokens.value);
    } else {
        return Calculation_output{-1, tokens.error};
    }
    
}

void ExprEvaluator::debug_infix_eval(){
    for(;;){
        std::cout << "Input Eqn : ";
        std::string inp;
        std::getline(std::cin,inp);
        std::cout << "\nThe input was: " << inp << "\n";
        Calculation_output res = evaluate_infix(inp);
        
        if (res.is_okay()) {
            std::cout << "The result is : " << res.value << "\n";
            
        } else {
            std::cout << "Error encountered: " << res.error << "\n";
        }
        
    }
}

int ExprEvaluator::evaluate_infix_1(const std::string &expr, int level){
    
    std::string filled_expr = fill_variables(expr, level);
    Calculation_output output = evaluate_infix(filled_expr);

    CHECK(output.is_okay()) << "Error encountered: " + output.error;
    return output.value;
}
