#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <vector>
#include <string>
#include <cmath>
#include <stack>
#include <functional>
#include <map>
#include <limits>

class Expression {

public:
    Expression(const std::string &input);
    // Main expression evaluation function
    double Value();
    
private:
    // Resulting value after evaluation
    double expressionResult;
    // Input expression string
    std::string inputExpression;
    // Parser success flag
    bool parsingSuccess;
    // Input string parsed into operands and operators
    std::vector <std::string> tokens;
    // Tokens represented in Reverse Polish Notation
    std::vector <std::string> tokensRPN; 
    // Operations mapped to lambda functions
    std::map < const char, std::function<double(double, double)> > opTable {
        {'+', [](double a, double b) { return a + b; } },
        {'-', [](double a, double b) { return a - b; } },
        {'*', [](double a, double b) { return a * b; } },
        {'/', [](double a, double b) { return a / b; } },
        {'^', [](double a, double b) { return pow(a, b); } } 
    };

    // Parser function for tokenization of input string
    void ParseTokens();
    // Tokens to Reverse Polish Notation converter
    void CreateRPN();
    // Evaluation of expression
    void EvaluateRPN();

    /*
        Common functions
    */

    int  priority (const std::string &c);
    bool isNumber (const std::string &symbol);
    bool isDigit (const char &c);
    bool isAlpha (const char &c);
    bool isSpace (const char &c);
    bool isOperator  (const std::string &c);
    bool isDelimiter (const char &c);
};


#endif // EXPRESSION_HPP