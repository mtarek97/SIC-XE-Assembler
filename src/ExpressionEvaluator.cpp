#include "ExpressionEvaluator.h"

ExpressionEvaluator::ExpressionEvaluator()
{
    //ctor
}

SymbolInfo ExpressionEvaluator::evaluateExpression(string expression)
{
    SymbolTable symbolTable;
    // assuming it's a simple expression <operand><operation><operand>
    vector<string> operands = ValidatorUtilities::split(str,"[-/+*]");
    SymbolInfo firstOperand = symbolTable->search(operands[0]);
    SymbolInfo secondOperand = symbolTable->search(operands[1]);

    char operation = (expression.substr(operands[0].size(), 1))[0];

    //absolute expression (only absolute terms)
    if(firstOperand->getLocation() == -1 && secondOperand->getLocation() == -1){
        switch(operation){
        case '+':
            return SymbolInfo(firstOperand + secondOperand, 'a');
        case '-':
            return SymbolInfo(firstOperand - secondOperand, 'a');
        case '*':
            return SymbolInfo(firstOperand * secondOperand, 'a');
        case '/':
            return SymbolInfo(firstOperand / secondOperand, 'a');
        }
    }
    //absolute expression (pair of relative terms)
    if(firstOperand->getLocation() != -1 && secondOperand->getLocation() != -1){
        switch(operation){
        case '+':
            return SymbolInfo(-1, 'e');
        case '-':
            return SymbolInfo(firstOperand - secondOperand, 'a');
        case '*':
            return SymbolInfo(-1, 'e');
        case '/':
            return SymbolInfo(-1, 'e');
        }
    }

    //relative expression (an absolute term and a relative term respectively)
    if(firstOperand->getLocation() != -1 && secondOperand->getLocation() == -1){
        switch(operation){
     #include "SymbolInfo.h"
   case '+':
            return SymbolInfo(firstOperand + secondOperand, 'a');
        case '-':
            return SymbolInfo(firstOperand - secondOperand, 'a');
        case '*':
            return SymbolInfo(-1, 'e');
        case '/':
            return SymbolInfo(-1, 'e');
        }
    }
    //anything else
    return SymbolInfo(-1, 'e');
}
