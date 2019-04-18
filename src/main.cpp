#include "Expression.hpp"
#include <iostream>

void tests ();

int main () {
	tests ();
}


void tests () {
	Expression *expr = new Expression("32 ^4 +(11-(3 * 2 ) ) / 2");
	double result;

	result = expr->Value();
	if (!std::isnan(result))
		std::cout << "Result of evaluation: " << result << std::endl;
	else 
		std::cout << "Evaluation not successful" << std::endl;
	delete expr;
    
    std::cout << std::endl;

    expr = new Expression("-32 ^-4 +(11-(3 * 2 ) ) / 2");
    result = expr->Value();
    if (!std::isnan(result))
        std::cout << "Result of evaluation: " << result << std::endl;
    else 
        std::cout << "Evaluation not successful" << std::endl;
    delete expr;

    std::cout << std::endl;

    expr = new Expression("(4+3)*2-(7+3)/2");
	result = expr->Value();
	if (!std::isnan(result))
		std::cout << "Result of evaluation: " << result << std::endl;
	else 
		std::cout << "Evaluation not successful" << std::endl;
	delete expr;

    std::cout << std::endl;
    
    expr = new Expression("(4+3)*2-(7+3)/0");
	result = expr->Value();
	if (!std::isnan(result))
		std::cout << "Result of evaluation: " << result << std::endl;
	else 
		std::cout << "Evaluation not successful" << std::endl;
    delete expr;
}