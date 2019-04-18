#include <string>
#include <stack>
#include <vector>
#include <iostream>
#include "Expression.hpp"
#include <stdio.h>
using namespace std;

const string errorDelimiter = " -> ";

typedef enum tokenTypes {
  None = 0,
  Operand = 1,
  Operator = 2,
  LeftParan = 3,
  RightParan = 4,
} tokenTypes;
/*
    Expression Class Implementation
*/

#define p(X) cout << (X) << endl;

Expression :: Expression (const string input) : inputExpression (input) {
  tokens.clear();
  tokensRPN.clear();
}

void Expression :: ParseTokens () {
  int i= 0, inputLength = inputExpression.size();
  int paranCnt = 0;
  string tempToken;
  char currentSymbol;
  char prevSymbol;
  tokenTypes lastTokenType = None;

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

  int tokensCount = tokens.size(), j;
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
  int i, tokensCount = tokens.size();
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

  for (i = 0; i < tokensRPN.size(); ++i) {
    cout << tokensRPN[i] << " ";
  }
  cout << endl;
}

void Expression :: EvaluateRPN () {
  CreateRPN();
  int i, tokensRPNCount = tokensRPN.size();
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
      double currentEval = opTable[currentRPNToken[0]](lOperand, rOperand);
      evalResult.push(currentEval);
    }
  }

  // TODO
  if (!evalResult.empty()) {
    expressionResult = evalResult.top();
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
  bool isNumber = false;
  for(unsigned int i = 0; i < symbol.size(); i++)
    if(!_isDigit(symbol[i]))
      return false;
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


