#include "SymbolInfo.h"
#include "ValidatorUtilities.h"


#ifndef EXPRESSIONEVALUATOR_H
#define EXPRESSIONEVALUATOR_H

using namespace std;


class ExpressionEvaluator
{
    public:
        ExpressionEvaluator();
        SymbolInfo evaluateExpression(std::string expression);

    protected:

    private:
};

#endif // EXPRESSIONEVALUATOR_H
