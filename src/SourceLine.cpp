#include "SourceLine.h"

SourceLine::SourceLine()
{

}
void SourceLine::setComment(string comment)
{
    this->comment = comment;
}
void SourceLine::setLable(string lable)
{
    this->lable = lable;
}
void SourceLine::setOperation(string operation)
{
    this->operation = operation;
}
void SourceLine::setOperand(string operand)
{
    this->operand = operand;
}
void SourceLine::setLocationCounter(int locationCounter)
{
    this->locationCounter = locationCounter;
}
void SourceLine::setNextInstruction(int nextInstruction) {
    this->nextInstruction = nextInstruction;
}

string SourceLine::getComment()
{
    return this->comment;
}
string SourceLine::getLable()
{
    return this->lable;
}
string SourceLine::getOperation()
{
    return this-> operation;
}
string SourceLine::getOperand()
{
    return this->operand;
}

int SourceLine::getLocationCounter()
{
    return this->locationCounter;
}

int SourceLine::getNextInstruction() {
    return this->nextInstruction;
}

void SourceLine::setHasObjCode(bool hasObjCode)
{
    this->hasObjCode = hasObjCode;
}
bool SourceLine::getHasObjCode()
{
    return this->hasObjCode;
}

void SourceLine::setIsValid(bool isValid)
{
    this->isValid = isValid;
}
bool SourceLine::getIsValid()
{
    return this->isValid;
}
void SourceLine::setErrorMessage(string errMsg)
{
    this->errorMessage = errMsg;
}
string SourceLine::getErrorMessage()
{
    return this->errorMessage;
}



