#include "Expression.hpp"
#include <iostream>

void tests ();

int main () {
  tests ();
}


void tests () {
  Expression *expr = new Expression();
  double result;

  expr->SetExpression("32 ^4 +(11-(3 * 2 ) ) / 2");
  expr->EvaluateRPN();
  result = expr->GetEvalResult();
  if (!std::isnan(result))
    std::cout << "Result of evaluation: " << result << std::endl;
  else 
    std::cout << "Evaluation not successful" << std::endl;

  std::cout<< std::endl;

  expr->SetExpression("(4+3)*2-(7+3)/2");
  expr->EvaluateRPN();
  result = expr->GetEvalResult();
  if (!std::isnan(result))
    std::cout << "Result of evaluation: " << result << std::endl;
  else 
    std::cout << "Evaluation not successful" << std::endl;

  std::cout<< std::endl;

  expr->SetExpression("(4+3)*2-(7+3)/0");
  expr->EvaluateRPN();
  result = expr->GetEvalResult();
  if (!std::isnan(result))
    std::cout << "Result of evaluation: " << result << std::endl;
  else 
    std::cout << "Evaluation not successful" << std::endl;
}