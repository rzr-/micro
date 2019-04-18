#include <string>
#include <stack>
#include <vector>
#include <iostream>
#include "Expression.hpp"
#include <stdio.h>
#include <limits>
using namespace std;

const string errorDelimiter = " -> ";

/*
    Expression Class Implementation
*/

#define p(X) cout << (X) << endl;

Expression :: Expression () {
  expressionResult = 0.0;
  tokens.clear();
  tokensRPN.clear();
}

Expression :: Expression (const string input) : inputExpression(input) {
  expressionResult = numeric_limits<double>::quiet_NaN();
  evaluationSuccess = false;
  tokens.clear();
  tokensRPN.clear();
  cout << "Expression: \"" << input << "\" initialized\n";
}

void Expression :: SetExpression (const string input) {
  inputExpression = input;
  expressionResult = std::numeric_limits<double>::quiet_NaN();
  evaluationSuccess = false;
  tokens.clear();
  tokensRPN.clear();
  cout << "Expression: \"" << input << "\" initialized\n";
}

void Expression :: ParseTokens () {
  uint i= 0, inputLength = inputExpression.size();
  int paranCnt = 0;
  string tempToken;
  char currentSymbol;

  for (i = 0; i < inputLength; ++i) {
    currentSymbol = inputExpression[i];

    if (_isAlpha(currentSymbol)) {
      cout << "Alphabet character detected:\n";
      cout << inputExpression.insert(i, errorDelimiter) << endl;
      cout << "Aborting.\n";
      exit (0);
    }

    if (_isDigit(currentSymbol)) {
      tempToken += currentSymbol;
    }

    else if (currentSymbol == '.') {
      tempToken += currentSymbol;
    }

    else if (isDelimiter(currentSymbol)) {
      if (currentSymbol == '(')
        paranCnt++;

      if (currentSymbol == ')') 
        paranCnt--;

      if (!tempToken.empty()) {
        tokens.push_back(tempToken);
        tempToken.clear();
      }
      tokens.push_back(string(1, currentSymbol));
    }
    else if (_isSpace(currentSymbol) && !tempToken.empty()) {
      tokens.push_back(tempToken);
      tempToken.clear();
    }
    else if (!_isSpace(currentSymbol) && currentSymbol != '.') {
      cout << "Unrecognized symbol: " << currentSymbol << endl;
      cout << inputExpression.insert(i, errorDelimiter) << endl;
      cout << "Aborting.\n";
      exit (0);
    }
    if (i == inputLength-1 && !tempToken.empty()) {
      tokens.push_back(tempToken);
      //checkToken(tempToken);
    }
  }
  if (paranCnt > 0) {
    cout << "Closing parenthesis missing.\nAborting.\n";
    exit (0);
  }
  if (paranCnt < 0) {
    cout << "Opening parenthesis missing.\nAborting.\n";
    exit (0);
  }

  uint tokensCount = tokens.size();
  tempToken = tokens.front();
  bool tempIsOp = isOperator(tempToken);
  if ( tempIsOp ) {
    if (tempToken != "+" || tempToken != "-")
      tokens.insert(tokens.begin(), "0");
    else {
      cout << "Expression cannot start with operator " << tempToken << "\nAborting.\n";
      exit (0);
    }
  }

  tempToken = tokens.back();
  if ( isOperator(tempToken) ) {
    cout << "Expression cannot end with operator " << tempToken << "\nAborting.\n";
    exit (0);
  }

  for (i = 1; i < tokensCount; ++i) {
    if (_isNumber(tokens[i-1])  && _isNumber(tokens[i])) {
      cout << "Two operands in a row: " << tokens[i-1] << " " << tokens[i] << "\nAborting.\n";
      exit (0); 
    }
    if (isOperator(tokens[i-1]) && isOperator(tokens[i])) {
      cout << "Two operators in a row: " << tokens[i-1] << " " << tokens[i] << "\nAborting.\n";
      exit (0);
    }
  }
}

void Expression :: CreateRPN () {
  ParseTokens();
  uint i, tokensCount = tokens.size();
  stack <string> s; 

  for(i = 0; i < tokensCount; ++i) {
    if(_isNumber(tokens[i])) {
        tokensRPN.push_back(tokens[i]);
    }
    if(tokens[i] == "(") {
      s.push(tokens[i]);
    }
    if(tokens[i] == ")") {
      while(!s.empty() && s.top() != "(") {
        tokensRPN.push_back(s.top());
        s.pop();
      }
      s.pop();
    }
    if(isOperator(tokens[i]) == true) {
      while(!s.empty() && _priority(s.top()) >= _priority(tokens[i])) {
        tokensRPN.push_back(s.top());
        s.pop();
      }
      s.push(tokens[i]);
    }
  }

  while(!s.empty()) {
    tokensRPN.push_back(s.top());
    s.pop();
  }

  cout << "Reverse Polish Notation of input expression: ";
  for (i = 0; i < tokensRPN.size(); ++i) {
    cout << tokensRPN[i] << " ";
  }
  cout << endl;
}

void Expression :: EvaluateRPN () {
  CreateRPN();
  uint i, tokensRPNCount = tokensRPN.size();
  stack <double> evalResult;

  for(i = 0; i < tokensRPNCount; i++) {
    string currentRPNToken = tokensRPN[i];
    // If s contains any char that is NOT an op,
    // consider it a number by default.
    if (currentRPNToken.find_first_not_of("+*/-^") != currentRPNToken.npos) {
      evalResult.push(stod(currentRPNToken.c_str()));
    } 
    else {
      double rOperand = evalResult.top(); evalResult.pop();
      double lOperand = evalResult.top(); evalResult.pop();
      if (rOperand == 0 and currentRPNToken[0] == '/') {
        evalResult = stack<double>();
        cout << "Division by zero\n";
        break;
      }
      double currentEval = opTable[currentRPNToken[0]](lOperand, rOperand);
      evalResult.push(currentEval);
    }
  }

  if (!evalResult.empty()) {
    expressionResult = evalResult.top();
    evaluationSuccess = true;
  }
  else {
    expressionResult = numeric_limits<double>::quiet_NaN();
    evaluationSuccess = false;
  }
}

double Expression :: GetEvalResult () {
  return expressionResult;   
}


/*
    Common functions
*/

int _priority (const string &c) {
  if(c == "^") return 3;
  if(c == "*" || c == "/") return 2;
  if(c == "+" || c == "-") return 1;
  else return 0;
}

bool _isNumber (const string &symbol) {
  try {
    stod (symbol);
  }
  catch (...) {
    return false;
  }
  return true;
}
bool _isDigit (const char &c) {
  return std::isdigit(static_cast<unsigned char>(c));
}

bool _isAlpha (const char &c) {
  return std::isalpha(static_cast<unsigned char>(c));
}

bool _isSpace (const char &c) {
  return std::isspace(static_cast<unsigned char>(c));
}
bool isOperator (const string &c) {
  return (c == "+" || c == "-" || c == "*" || c == "/" || c == "^");
}
bool isDelimiter (const char &c) {
  return (c == '+' || c == '-' || c == '*' || c == '/' ||c == '^' || c == '(' || c == ')');
}


