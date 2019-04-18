#include "Expression.hpp"
#include <iostream>

int main () {
  std::string input = "-32. ^4 +(11-(3 * 2 ) ) / 2";
  // input = "(4+3.0)*2-(7+3)/2";
  // input = "";
  if (input.empty()) {
    std:: cout << "Expression string empty\n"; 
    exit (0);
  }
  Expression Expression(input);
  Expression.EvaluateRPN();
  std::cout << Expression.GetEvalResult() << std::endl;
}
