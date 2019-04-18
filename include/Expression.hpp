#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <vector>
#include <string>
#include <cmath>
#include <stack>
#include <functional>
#include <map>

class Expression {
  // Resulting value after evaluation
  double expressionResult;
  // Input expression string
  std::string inputExpression;
  //
  bool evaluationSuccess = false;
  // Input string parsed into operands and operators
  std::vector <std::string> tokens;
  // Tokens represented in Reverse Polish Notation
  std::vector <std::string> tokensRPN; 
  // Operations mapped to lambda functions
  std::map < const char, std::function<double(double, double)> > opTable {
    {'+',[](double a, double b){ return a + b;} },
    {'-',[](double a, double b){ return a - b;} },
    {'*',[](double a, double b){ return a * b;} },
    {'/',[](double a, double b){ return a / b;} },
    {'^',[](double a, double b){ return pow(a, b);} } };

  // Parser function for tokenization
  void ParseTokens ();
  // Tokens to Reverse Polish Notation converter
  void CreateRPN ();
  
public:
  Expression();
  Expression(const std::string input);
  void SetExpression(const std::string input);
  // Main expression evaluation function
  void EvaluateRPN();
  double GetEvalResult();
};


/*
    Common functions
*/

int  _priority (const std::string &c);
bool _isNumber (const std::string &symbol);
bool _isDigit (const char &c);
bool _isAlpha (const char &c);
bool _isSpace (const char &c);
bool isOperator  (const std::string &c);
bool isDelimiter (const char &c);


#endif // EXPRESSION_HPP