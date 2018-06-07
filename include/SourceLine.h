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
        int getNextInstruction();
        void setComment(string comment);
        void setLable(string lable);
        void setOperation(string operation);
        void setOperand(string operand);
        void setByteFormat(int byteFormat);
        void setNextInstruction(int nextInstruction);

    private:
        string lable="", operation="", operand="", comment="";
        int byteFormat = 0,nextInstruction=0;
};

#endif // SOURCELINE_H
