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


