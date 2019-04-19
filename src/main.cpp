#include "Expression.hpp"
#include <iostream>

void tests ();
bool isEqual(double x, double y);

double quiet_nan = std::numeric_limits<double>::quiet_NaN();

const std::vector <std::tuple<std::string, double>> test_cases {
    { "1920*720*4*              2",                     double(11059200) },
    { "(64+16+24+4+16+16+24+16)/40",      double(4.5) },
    { "32 ^ 4 + (11-(3 * 2)) / 332",      double(1048576.01506024) },
    { "32 ^-4 + (11-(3 * 2)) / 332",      double(0.01506119) },
    { "32 ^ (-4) + (11-(3 * 2)) / 332",   double(0.01506119) },
    { "(32) ^ (-4) + (11-(3 * 2)) / 332", double(0.01506119) },
    { "32 ^ 4 + (11-(3 * 2)) / 2",        double(1048578.5) },
    { "(4+3)*2-(7+3)/2",   double(9) },
    { "(4+3.)*2-(7+3)/2",  double(9) },
    { "(4+3.0)*2-(7+3)/2", double(9) },
    // Error cases
    { "(4+3)*2-(7+3)/0",     quiet_nan },
    { "(4+3)-*2-(7+3)/0",    quiet_nan },
    { "*(4+3)*2-(7+3)/0",    quiet_nan },
    { "((=4+3)*2-(7+3)/0",   quiet_nan },
    { "(4(+3)*2-(7+3)/0",    quiet_nan },
    { "(4(+3))*2-(7+3)/0",   quiet_nan },
    
    { "(4+3)*2-(7+3)/2.9",   quiet_nan },
    { "(4+3...)*2-(7+3)/2",  quiet_nan },
    { "(4+3.2.)*2-(7+3)/2",  quiet_nan },
    { "(4+3.2.2)*2-(7+3)/2", quiet_nan },
    { "(4+3. .)*2-(7+3)/2",  quiet_nan },
    { "(4+3. .0)*2-(7+3)/2", quiet_nan },
    { "(4+.3)*2-(7+3)/2",    quiet_nan }
};

int main () {
    tests ();
}

void tests () {
    std::cout.precision(12);
    
    for (auto& [exprString, exprResult]: test_cases) {
        Expression *expr = new Expression(exprString);
        double evalResult = expr->Value();

        if (!std::isnan(evalResult))  {
            std::cout << "Evaluation Successful" << std::endl;
            std::cout << "Expected result:\t" << exprResult << std::endl;
            std::cout << "Result of evaluation:\t" << evalResult << std::endl;
            if (isEqual(exprResult, evalResult)) {
                std::cout << "Test Successful" << std::endl;
            }
            else {
                std::cout << "Test Failed" << std::endl;
            }
        }
        else {
         std::cout << "Evaluation not successful" << std::endl;
         std::cout << "Test Failed" << std::endl;
        }
        delete expr;
        std::cout << std::endl;
    }
}

bool isEqual(double x, double y) {
    int _x = (x - int(x)) * 100000, 
        _y = (y - int(y)) * 100000;
        
    if (_x == _y) 
        return true;
    return false;
}