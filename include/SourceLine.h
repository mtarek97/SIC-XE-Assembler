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
        bool getIsValid();
        bool getHasObjCode();
        string getErrorMessage();
        int getLocationCounter();
        int getNextInstruction();
        bool getContainsExpression();

        void setComment(string comment);
        void setLable(string lable);
        void setOperation(string operation);
        void setOperand(string operand);
        void setNextInstruction(int nextInstruction);
        void setIsValid(bool isValid);
        void setHasObjCode(bool hasObjCode);
        void setErrorMessage(string error);
        void setLocationCounter(int locationCounter);
        void setContainsExpression(bool containsExp);


    private:
        string lable="", operation="", operand="", comment="";
        int nextInstruction=0, locationCounter = 0;
        bool isValid = true,hasObjCode = true,containsExpression = false;
        string errorMessage = "";


};

#endif // SOURCELINE_H
