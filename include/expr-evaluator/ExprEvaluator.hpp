//
//  ExprEvaluator.hpp
//  expr_evaluator


#ifndef ExprEvaluator_hpp
#define ExprEvaluator_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

enum class T_Type: char {
    
    NUMBER = 'n',
    LEFT_PARENTHESIS = '(',
    RIGHT_PARENTHESIS = ')',
    PLUS = '+',
    MINUS = 'm',
    U_MINUS = 'u', //The unary minus (eg: -3, 1 + -5)
    MULTIPLY = '*',
    DIVIDE = '/'
    
    };
    
    struct Token {
        T_Type type;
        int val; //For T_Type number
    };
    
    typedef std::vector<Token> Expr_Token;
    
    struct Tokenizer_output {
        Expr_Token value;
        std::string error;
        bool is_okay() {
            return error.empty(); //use for testing as well
        }
    };
    
    struct Calculation_output {
        int value;
        std::string error;
        bool is_okay() {
            return error.empty(); //use for testing as well
        }
        
    };
    
    
    class ExprEvaluator {
        
    private:
        
        //Helper Functions
        
        static bool is_operator(T_Type t);
        static int operator_weight(T_Type t);
        static bool is_space(char c);
        static bool is_digit(char c);
        static void fix_negation(Expr_Token& exp);
        static bool is_variable(char c);
        static bool replace_str(std::string& str, const std::string& from, const std::string& to);
        static std::string fill_variables(const std::string& expr, int level);
        
        //Pop function for vector
        template<class T>
        static T pop(std::vector<T>& stack);
        
        //---------------------Converters---------------------------------//
        
        //Tokenizer: Converts string expr to tokens
        static Tokenizer_output tokenizer(const std::string& s);
        //Infix expr to Postfix expr (easier calculation)
        static Tokenizer_output infix_to_postfix(const Expr_Token& expr);
        
        //PostFix Token evaluator
        static Calculation_output evaluate_postfix(const Expr_Token& exp);
        
        //Infix Token evaluator
        //exp isnt const as this function will have to modify it
        static Calculation_output evaluate_infix(Expr_Token expr);
        
        
    public:
        
        //PostFix String evaluator
        static Calculation_output evaluate_postfix(const std::string& expr);
        
        //Infix String evaluator
        static Calculation_output evaluate_infix(const std::string& expr);
        
        
        //-------------------FUNCTIONS FOR SPELLS-----------------------------
        //
        //Infix Expr evaluate with one variable (level)
        static int evaluate_infix_1(const std::string& expr, int level);
        
        
        //--------------------------------Debug Functions --------------------
        static void debug_postfix_eval();
        static void debug_infix_eval();
        
        
    };
    
#endif /* ExprEvaluator_hpp */
