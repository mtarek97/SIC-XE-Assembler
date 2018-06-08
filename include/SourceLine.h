#include <bits/stdc++.h>
#ifndef SOURCELINE_H
#define SOURCELINE_H

using namespace std;

class SourceLine
{
    public:
        SourceLine();
        string getComment();
        string getLable();
        string getOperation();
        string getOperand();
        int getByteFormat();
        bool getIsValid();
        bool getHasObjCode();
        string getErrorMessage();
        bool getContainsExpression();

        int getNextInstruction();
        void setComment(string comment);
        void setLable(string lable);
        void setOperation(string operation);
        void setOperand(string operand);
        void setByteFormat(int byteFormat);
        void setNextInstruction(int nextInstruction);
        void setIsValid(bool isValid);
        void setHasObjCode(bool hasObjCode);
        void setErrorMessage(string error);
        void setContainsExpression(bool containsExp);

    private:
        string lable="", operation="", operand="", comment="";
        int byteFormat = 0,nextInstruction=0;
        bool isValid = true,hasObjCode = true,containsExpression = false;
        string errorMessage = "";
};

#endif // SOURCELINE_H
