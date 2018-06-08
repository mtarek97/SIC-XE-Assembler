#include "ValidatorUtilities.h"

using namespace std;

const std::string ValidatorUtilities::directives[] = {"BYTE","WORD","RESB","RESW","START","END","EQU","ORG","BASE","LTORG"};
OpCodeTable* ValidatorUtilities::opCodeTable = OpCodeTable::getOpTable();

// private constructor --static class
ValidatorUtilities::ValidatorUtilities() {}


vector<string> ValidatorUtilities::split(string str, string regexString)
{
    vector<string> tokens;
    regex rgx(regexString);
    regex_token_iterator<string::iterator> iter(str.begin(), str.end(), rgx, -1);
    regex_token_iterator<string::iterator> end;

    while (iter != end)
    {
        tokens.push_back(*iter);
        iter++;
    }
    return tokens;
}

bool ValidatorUtilities::isSymbol(string str,int maxLength)
{
    if(str.size() > maxLength)
    {
        return false;
    }
    if(isdigit(str[0]))
    {
        return false;
    }
    for(int i=0; i<str.size(); i++)
    {
        if(!isalpha(str[i]) && !isdigit(str[i]) && !(str[i]=='_'))
        {
            return false;
        }
    }
    return true;
}

bool ValidatorUtilities::isReservedKeyword(string str)
{
    if(isDirective(str))
    {
        return true;
    }
    OpInfo info = opCodeTable->getInfo(str);
    if(info.getOpCode() != OpInfo::NOT_FOUND)
    {
        return true;
    }
    return false;
}

bool ValidatorUtilities::isHexAddress(string str, int maxLength)
{
    if(str.size()>maxLength)
    {
        return false;
    }
    for(int i=0; i<str.size(); i++)
    {
        if(!isxdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

bool ValidatorUtilities::isDirective(string str)
{
    if(find(directives,directives+10,str) == directives+10)
    {
        return false;
    }
    return true;
}

bool ValidatorUtilities::isDecimalNumber(string str, int maxDigitsCount, bool canBeNegative=false)
{
    string absolute = str;
    bool isNegative = false;
    if(str[0]=='-')
    {
        absolute = str.substr(1);
        isNegative = true;
    }
    if(isNegative && !canBeNegative)
    {
        return false;
    }
    if(absolute.size() > maxDigitsCount)
    {
        return false;
    }
    for(int i=0; i<absolute.size(); i++)
    {
        if(!isdigit(absolute[i]))
        {
            return false;
        }
    }
    return true;
}

// returns true if exp string has two or more operands, false otherwise
bool ValidatorUtilities::isExpression(string str,bool canBeGeneralExpression)
{
    vector<string> operands = split(str,"[-/+*]");
    for(string operand : operands)
    {
        if(isReservedKeyword(operand) || operand=="")
        {
            return false;
        }
        if(!isSymbol(operand,8) && !isDecimalNumber(operand,7,false))
        {
            return false;
        }
    }
    if(!canBeGeneralExpression)
    {
        return operands.size()==2;
    }
    return operands.size()>=2;
}

// Literals are constants preceded by = (e.g., =W'-1234' , =C’The result’ , and =X’FF5A9EBD’
bool ValidatorUtilities::isLiteral(string str)
{
    if(str[0]=='=' && str.size()>=4 && str[2]== '\'' && str[str.size()-1]=='\'')
    {
        char literalType = str[1];
        string value = str.substr(3,str.size()-4);
        bool validLiteral = false;
        switch(literalType){
        case 'X' :
            validLiteral = isHexAddress(value,100);
            break;
         case 'C' :
            validLiteral = true;
            break;
         case 'W' :
            validLiteral = isDecimalNumber(value,50,true);
            break;
         default :
            validLiteral = false;
        }
        return validLiteral;
    }
    else
    {
        return false;

    }
}

SourceLine* ValidatorUtilities::toUpperCase(SourceLine* srcLine)
{
    string label = srcLine->getLable();
    string operand = srcLine->getOperand();
    string operation = srcLine->getOperation();
    string comment = srcLine->getComment();
    transform(label.begin(), label.end(), label.begin(), ::toupper);
    transform(operand.begin(), operand.end(), operand.begin(), ::toupper);
    transform(operation.begin(), operation.end(), operation.begin(), ::toupper);
    transform(comment.begin(), comment.end(), comment.begin(), ::toupper);
    srcLine->setLable(label);
    srcLine->setOperand(operand);
    srcLine->setOperation(operation);
    srcLine->setComment(comment);
    return srcLine;
}

