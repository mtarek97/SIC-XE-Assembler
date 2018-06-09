#include <SymbolTable.h>
#include "ExpressionEvaluator.h"
#include<bits/stdc++.h>
#include <SymbolTable.h>
#include "SymbolInfo.h"

using namespace std;

ExpressionEvaluator::ExpressionEvaluator()
{
    //ctor
}

SymbolInfo ExpressionEvaluator::evaluateExpression(std::string expression)
{
    SymbolTable* symbolTable = SymbolTable::getSymbolTable();
    // assuming it's a simple expression <operand><operation><operand>
    vector<std::string> operands = ValidatorUtilities::split(expression,"[-/+*]");
    int h = operands.size();
    SymbolInfo firstOperand = symbolTable->search(operands[0]);
    SymbolInfo secondOperand = symbolTable->search(operands[1]);

    char operation = (expression.substr(operands[0].size(), 1))[0];

    //absolute expression (only absolute terms)
    if(firstOperand.getLocation() == -1 && secondOperand.getLocation() == -1){
        switch(operation){
        case '+':
            return SymbolInfo(stoi(operands[0]) + stoi(operands[1]), 'a');
        case '-':
            return SymbolInfo(stoi(operands[0]) - stoi(operands[1]), 'a');
        case '*':
            return SymbolInfo(stoi(operands[0]) * stoi(operands[1]), 'a');
        case '/':
            return SymbolInfo(stoi(operands[0]) / stoi(operands[1]), 'a');
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
            return SymbolInfo(firstOperand.getLocation() + stoi(operands[1]), 'r');
        case '-':
            return SymbolInfo(firstOperand.getLocation() - stoi(operands[1]), 'r');
        case '*':
            return SymbolInfo(-1, 'e');
        case '/':
            return SymbolInfo(-1, 'e');
        }
    }
    //anything else
    return SymbolInfo(-1, 'e');
}
