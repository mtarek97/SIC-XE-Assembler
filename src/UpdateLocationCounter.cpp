#include "UpdateLocationCounter.h"
#include<SourceLine.h>
#include<OpInfo.h>
#include<OpCodeTable.h>
#include<SourceProgram.h>
#include<ValidatorUtilities.h>
#include<SyntaxValidator.h>
#include <ExpressionEvaluator.h>
#include<SymbolInfo.h>
#include <SymbolTable.h>
UpdateLocationCounter::UpdateLocationCounter()
{
    //ctor
}
pair<int,string> UpdateLocationCounter::setLocationCounter(int locationCounter, SourceLine sourceLine)
{
    string error = "";
    SymbolTable* symbolTable = SymbolTable::getSymbolTable();
    OpCodeTable* opCodeTable = OpCodeTable::getOpTable();
    OpInfo opinfo = opCodeTable->getInfo(SourceProgram::getUpper(sourceLine.getOperation()));
    if(opinfo.getOpCode() != opinfo.NOT_FOUND)
    {
        locationCounter+=opinfo.getFormatBytes();
    }
    else if(sourceLine.getOperation()[0] == '+')
    {
        string withOutPlus = sourceLine.getOperation();
        withOutPlus.erase(withOutPlus.begin());
        if(opCodeTable->getInfo(SourceProgram::getUpper(withOutPlus)).getOpCode() != opinfo.NOT_FOUND)
        {
            locationCounter+=4;
        }
    }
    else
    {
        string operation = SourceProgram::getUpper(sourceLine.getOperation());
        if(operation == "WORD")
        {

            int numberOfWords = ValidatorUtilities::split(sourceLine.getOperand(), "[,]").size();
            locationCounter=locationCounter + 3 * numberOfWords;
        }
        else if(operation == "BYTE")
        {
            if(sourceLine.getOperand()[0]=='C' || sourceLine.getOperand()[0]=='c')
                locationCounter = locationCounter + (sourceLine.getOperand().length()) - 3;
            else
                locationCounter = locationCounter + ((sourceLine.getOperand().length() - 3 + 1 )/2);
        }
        else if(operation == "RESB")
        {
            locationCounter = locationCounter + stoi(sourceLine.getOperand());

        }
        else if(operation == "RESW")
        {
            locationCounter = locationCounter + 3 * stoi(sourceLine.getOperand());
        }
        else if(operation == "ORG")
        {


            if(sourceLine.getContainsExpression())
            {

                ExpressionEvaluator evaluate;
                SymbolInfo result = evaluate.evaluateExpression(sourceLine.getOperand());
                if(result.getLocation() != -1)
                    locationCounter = result.getLocation();

                else if(result.getType() == 'e')
                    error = "Not Absolute and Not Relative";
                else if(result.getType() == 'u')
                    error = "This symbol is not exist until now";

            }
            else if(symbolTable->hashtable.count(sourceLine.getOperand()) != 0)
            {
                locationCounter = symbolTable->hashtable[sourceLine.getOperand()].getLocation();
            }

            else if(sourceLine.getOperand() == "*")
            {
                // ha ha ha ha
            }
            else if(isInt(sourceLine.getOperand()))
            {
                locationCounter = std::stoi(sourceLine.getOperand());
                std::stringstream stream;
                stream << locationCounter;
                stream >> std::hex >> locationCounter;

            }
            else if(sourceLine.getOperand() != "*")
                error = "This symbol is not exist until now";

        }

        else if(operation == "EQU")
        {

            if(sourceLine.getContainsExpression())
            {
                ExpressionEvaluator evaluate;

                SymbolInfo result = evaluate.evaluateExpression(sourceLine.getOperand());
                if(result.getLocation() != -1)
                {
                    //   cout<<"here"<<evaluate.evaluateExpression(sourceLine.getOperand()).getLocation();
                    symbolTable->insert(sourceLine.getLable(), result.getLocation());
                }
                else if(result.getType() == 'e')
                    error = "Not Absolute and Not Relative";
                else if(result.getType() == 'u')
                    error = "This symbol is not exist until now";

            }
            else if(symbolTable->hashtable.count(sourceLine.getOperand()) != 0)
            {
                symbolTable->insert(sourceLine.getLable(), symbolTable->hashtable[sourceLine.getOperand()].getLocation());
            }
            else if(sourceLine.getOperand() == "*")
            {
                symbolTable->insert(sourceLine.getLable(), locationCounter);
            }
            else if(isInt(sourceLine.getOperand()))
            {
                int x = std::stoi(sourceLine.getOperand());
                std::stringstream stream;
                stream << x;
                stream >> std::hex >> x;
                symbolTable->insert(sourceLine.getLable(), x);
            }
            else
                error = "This symbol is not exist until now";

        }
        else if(sourceLine.getLable() == "*" && sourceLine.getOperation()[0] == '=')
        {
            if(toupper(sourceLine.getOperation()[1])=='C')
                locationCounter = locationCounter + (sourceLine.getOperation().length()) - 4;
            else
                locationCounter = locationCounter + ((sourceLine.getOperation().length() - 4 + 1 )/2);
        }
    }
    return make_pair(locationCounter, error);
}


int UpdateLocationCounter::detectStart(int locationCounter,SourceLine sourceLine)
{
    locationCounter = std::stoi(sourceLine.getOperand());
    std::stringstream stream;
    stream << locationCounter;
    stream >> std::hex >> locationCounter;

    return locationCounter;
}
bool UpdateLocationCounter::isInt(std::string operand)
{
    int i;
    for(i=0; i<operand.size(); i++)
    {
        if(operand[i] >= '0' && operand[i] <= '9')
        {
            continue;
        }
        return false;
    }
    return true;
}

