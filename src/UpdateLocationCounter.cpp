#include "UpdateLocationCounter.h"
#include<SourceLine.h>
#include<OpInfo.h>
#include<OpCodeTable.h>
#include<SourceProgram.h>
#include<ValidatorUtilities.h>
#include<SyntaxValidator.h>
UpdateLocationCounter::UpdateLocationCounter()
{
    //ctor
}
int UpdateLocationCounter::setLocationCounter(int locationCounter, SourceLine sourceLine) {
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
        if(opCodeTable->getInfo(SourceProgram::getUpper(withOutPlus)).getOpCode() != opinfo.NOT_FOUND){
                locationCounter+=4;
        }
        }
        else
        {
            string operation = SourceProgram::getUpper(sourceLine.getOperation());
            if(operation == "WORD") {

               int numberOfWords = ValidatorUtilities::split(sourceLine.getOperand(), "[,]").size();
                locationCounter=locationCounter + 3 * numberOfWords;
            }
            else if(operation == "BYTE") {
                if(sourceLine.getOperand()[0]=='C' || sourceLine.getOperand()[0]=='c')
                  locationCounter = locationCounter + (sourceLine.getOperand().length()) - 3;
                else
                  locationCounter = locationCounter + ((sourceLine.getOperand().length() - 3 + 1 )/2);
            }
            else if(operation == "RESB") {
             locationCounter = locationCounter + stoi(sourceLine.getOperand());

            }
            else if(operation == "RESW") {
             locationCounter = locationCounter + 3 * stoi(sourceLine.getOperand());
            }
        }


            return locationCounter;
}


int UpdateLocationCounter::detectStart(int locationCounter,SourceLine sourceLine)
{
    locationCounter = std::stoi(sourceLine.getOperand(), nullptr, 2);
    return locationCounter;
}
