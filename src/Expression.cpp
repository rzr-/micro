#include <iostream>
#include "Expression.hpp"
using namespace std;

/*
        Expression Class Implementation
*/

Expression :: Expression (const string &input) 
            : inputExpression(input),
              parsingSuccess(false)
{
    expressionResult = std::numeric_limits<double>::quiet_NaN();
    cout << "Expression: \"" << input << "\" initialized\n";
}

double Expression :: Value () {
    if (!parsingSuccess) {
        try {
            ParseTokens();
        }
        catch (string errorMessage) {
            cout << errorMessage << endl;
        }
        if (parsingSuccess) {
            CreateRPN();
            try {
                EvaluateRPN();
            }
            catch (string errorMessage) {
                cout << errorMessage << endl;
            }
        }
    }
    return expressionResult;
}

void Expression :: ParseTokens () {
    uint i= 0, inputLength = inputExpression.size();
    int paranCount = 0;
    string tempToken;
    char currentSymbol;

    for (i = 0; i < inputLength; ++i) {
        currentSymbol = inputExpression[i];

        if (isAlpha(currentSymbol)) {
            throw string ("Alphabet character detected:\n" + inputExpression.insert(i, " -> "));
        }

        if (isDigit(currentSymbol) || currentSymbol == '.') {
            tempToken += currentSymbol;
        }

        else if (isDelimiter(currentSymbol)) {
            if (currentSymbol == '(')
                paranCount++;

            else if (currentSymbol == ')') 
                paranCount--;
            else if (currentSymbol == '-') {
                if (isDigit(inputExpression[i+1])) {
                    tokens.push_back("0");
                }
            }
            if (!tempToken.empty()) {
                tokens.push_back(tempToken);
                tempToken.clear();
            }
            tokens.push_back(string(1, currentSymbol));
        }
        else if (isSpace(currentSymbol) && !tempToken.empty()) {
            tokens.push_back(tempToken);
            tempToken.clear();
        }
        else if (!isSpace(currentSymbol) && currentSymbol != '.') {
            throw string ("Unrecognized symbol: " + inputExpression.insert(i, " -> "));
        } 
    }

    if (!tempToken.empty()) {
            tokens.push_back(tempToken);
    }
    if (paranCount > 0) {
        throw string ("Closing parenthesis missing.");
    }
    else if (paranCount < 0) {
        throw string ("Opening parenthesis missing.");
    }

    uint tokensCount = tokens.size();
    tempToken = tokens.front();
    bool tempIsOp = isOperator(tempToken);
    if ( tempIsOp ) {
        if (tempToken == "+" || tempToken == "-")
            tokens.insert(tokens.begin(), "0");
        else {
            throw string ("Expression cannot start with operator " + tempToken);
        }
    }

    tempToken = tokens.back();
    if ( isOperator(tempToken) ) {
        throw string ("Expression cannot end with operator " + tempToken);
    }

    for (i = 1; i < tokensCount; ++i) {
        if (isNumber(tokens[i-1])  && isNumber(tokens[i])) {
            throw string ("Two operands in a row: " + tokens[i-1] + " " +tokens[i]);
        }
        if (isOperator(tokens[i-1]) && isOperator(tokens[i])) {
            throw string ("Two operators in a row: " + tokens[i-1] + " " +tokens[i]);
        }
    }
    parsingSuccess = true;
}

void Expression :: CreateRPN () {
    uint i, tokensCount = tokens.size();
    stack <string> s; 

    for(i = 0; i < tokensCount; ++i) {
        if(isNumber(tokens[i])) {
                tokensRPN.push_back(tokens[i]);
        }
        else if(tokens[i] == "(") {
            s.push(tokens[i]);
        }
        else if(tokens[i] == ")") {
            while(!s.empty() && s.top() != "(") {
                tokensRPN.push_back(s.top());
                s.pop();
            }
            s.pop();
        }
        else if(isOperator(tokens[i]) == true) {
            while(!s.empty() && priority(s.top()) >= priority(tokens[i])) {
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
    for (string token : tokensRPN) {
        cout << token << " ";
    }
    cout << endl;
}

void Expression :: EvaluateRPN () {
    stack <double> evalResult;

    for (string currentRPNToken : tokensRPN) {
        // If s contains any char that is NOT an op,
        // consider it a number by default.
        if (currentRPNToken.find_first_not_of("+*/-^") != currentRPNToken.npos) {
            evalResult.push(stod(currentRPNToken.c_str()));
        } 
        else {
            double rOperand = evalResult.top(); evalResult.pop();
            double lOperand = evalResult.top(); evalResult.pop();
            if (rOperand == 0 and currentRPNToken[0] == '/') {
                throw string ("Division by zero");
            }
            double currentEval = opTable[currentRPNToken[0]](lOperand, rOperand);
            evalResult.push(currentEval);
        }
    }

    expressionResult = evalResult.top();
}

/*
        Common functions
*/

int Expression :: priority (const string &c) {
    if(c == "^") return 3;
    if(c == "*" || c == "/") return 2;
    if(c == "+" || c == "-") return 1;
    else return 0;
}

bool Expression :: isNumber (const string &symbol) {
    try {
        stod (symbol);
    }
    catch (...) {
        return false;
    }
    return true;
}
bool Expression :: isDigit (const char &c) {
    return std::isdigit(static_cast<unsigned char>(c));
}

bool Expression :: isAlpha (const char &c) {
    return std::isalpha(static_cast<unsigned char>(c));
}

bool Expression :: isSpace (const char &c) {
    return std::isspace(static_cast<unsigned char>(c));
}
bool Expression :: isOperator (const string &c) {
    return (c == "+" || c == "-" || c == "*" || c == "/" || c == "^");
}
bool Expression :: isDelimiter (const char &c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '(' || c == ')');
}


