#include "SymbolInfo.h"
#include "ValidatorUtilities.h"


#ifndef EXPRESSIONEVALUATOR_H
#define EXPRESSIONEVALUATOR_H
#include<bits/stdc++.h>
#include <string>

using namespace std;


class ExpressionEvaluator
{
    public:
        ExpressionEvaluator();
        SymbolInfo evaluateExpression(std::string expression);

    private:
        bool isInt(std::string operand);
};

#endif // EXPRESSIONEVALUATOR_H
