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
   // cout<<"-----"<<firstOperand.getLocation();

    char operation = (expression.substr(operands[0].size(), 1))[0];

    //absolute expression (only absolute terms)
    if((firstOperand.getLocation() == -1 && secondOperand.getLocation() == -1 && isInt(operands[0]) && isInt(operands[1]))
     ||(firstOperand.getType() == 'a' && secondOperand.getType() == 'a') ){
        if(firstOperand.getLocation() == -1 && secondOperand.getLocation() == -1 && isInt(operands[0]) && isInt(operands[1])){
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
        else if(firstOperand.getType() == 'a' && secondOperand.getType() == 'a'){
            switch(operation){
            case '+':
                return SymbolInfo(firstOperand.getLocation() + secondOperand.getLocation(), 'a');
            case '-':
                return SymbolInfo(firstOperand.getLocation() - secondOperand.getLocation(), 'a');
            case '*':
                return SymbolInfo(firstOperand.getLocation() * secondOperand.getLocation(), 'a');
            case '/':
                return SymbolInfo(firstOperand.getLocation() / secondOperand.getLocation(), 'a');
            }
        }
    }
    //absolute expression (pair of relative terms)
    if(firstOperand.getType() == 'r' && secondOperand.getType() == 'r'){
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

    //relative expression (a relative term and an absolute term respectively)
    if(firstOperand.getType() == 'r' && ((secondOperand.getLocation() == -1 && isInt(operands[1]))
                                       ||(secondOperand.getType() == 'a')) ){
        if(secondOperand.getLocation() == -1 && isInt(operands[1])){
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
        else if(secondOperand.getType() == 'a'){
            switch(operation){
            case '+':
                return SymbolInfo(firstOperand.getLocation() + secondOperand.getLocation(), 'r');
            case '-':
                return SymbolInfo(firstOperand.getLocation() - secondOperand.getLocation(), 'r');
            case '*':
                return SymbolInfo(-1, 'e');
            case '/':
                return SymbolInfo(-1, 'e');
            }
        }
    }

    //relative expression (an absolute and a relative term respectively) (must be addition)
    if(secondOperand.getType() == 'r' && ((firstOperand.getLocation() == -1 && isInt(operands[0]))
                                       ||(firstOperand.getType() == 'a')) ){
        if(firstOperand.getLocation() == -1 && isInt(operands[0])){
            switch(operation){
            case '+':
                return SymbolInfo(stoi(operands[0]) + secondOperand.getLocation(), 'r');
            case '-':
                return SymbolInfo(-1, 'e');
            case '*':
                return SymbolInfo(-1, 'e');
            case '/':
                return SymbolInfo(-1, 'e');
            }
        }
        else if(firstOperand.getType() == 'a'){
            switch(operation){
            case '+':
                return SymbolInfo(firstOperand.getLocation() + secondOperand.getLocation(), 'r');
            case '-':
                return SymbolInfo(-1, 'e');
            case '*':
                return SymbolInfo(-1, 'e');
            case '/':
                return SymbolInfo(-1, 'e');
            }
        }
    }
    //anything else
    return SymbolInfo(-1, 'e');
}


bool ExpressionEvaluator::isInt(std::string operand){
    int i;
    for(i=0; i<operand.size(); i++){
        if(operand[i] >= '0' && operand[i] <= '9'){
            continue;
        }
        return false;
    }
    return true;
}
