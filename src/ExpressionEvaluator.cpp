#include "ExpressionEvaluator.h"
#include "SymbolInfo.h"

using namespace std;

ExpressionEvaluator::ExpressionEvaluator()
{
    //ctor
}

SymbolInfo ExpressionEvaluator::evaluateExpression(std::string expression)
{
    SymbolTable symbolTable;
    // assuming it's a simple expression <operand><operation><operand>
    vector<std::string> operands = ValidatorUtilities::split(expression,"[-/+*]");
    SymbolInfo firstOperand = symbolTable.search(operands[0]);
    SymbolInfo secondOperand = symbolTable.search(operands[1]);

    char operation = (expression.substr(operands[0].size(), 1))[0];

    //absolute expression (only absolute terms)
    if(firstOperand.getLocation() == -1 && secondOperand.getLocation() == -1){
        switch(operation){
        case '+':
            return SymbolInfo(toi(operands[0]) + toi(operands[1]), 'a');
        case '-':
            return SymbolInfo(toi(operands[0]) - toi(operands[1]), 'a');
        case '*':
            return SymbolInfo(toi(operands[0]) * toi(operands[1]), 'a');
        case '/':
            return SymbolInfo(toi(operands[0]) / toi(operands[1]), 'a');
        }
    }
    //absolute expression (pair of relative terms)
    if(firstOperand.getLocation() != -1 && secondOperand.getLocation() != -1){
        switch(operation){
        case '+':
            return SymbolInfo(-1, 'e');
        case '-':
            return SymbolInfo(firstOperand.getLocation() - secondOperand.getLocation(), 'a');
        case '*':
            return SymbolInfo(-1, 'e');
        case '/':
            return SymbolInfo(-1, 'e');
        }
    }

    //relative expression (an absolute term and a relative term respectively)
    if(firstOperand.getLocation() != -1 && secondOperand.getLocation() == -1){
        switch(operation){
   case '+':
            return SymbolInfo(firstOperand.getLocation() + toi(operands[1]), 'r');
        case '-':
            return SymbolInfo(firstOperand.getLocation() - toi(operands[1]), 'r');
        case '*':
            return SymbolInfo(-1, 'e');
        case '/':
            return SymbolInfo(-1, 'e');
        }
    }
    //anything else
    return SymbolInfo(-1, 'e');
}
