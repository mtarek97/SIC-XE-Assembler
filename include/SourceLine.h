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
        void setComment(string comment);
        void setLable(string lable);
        void setOperation(string operation);
        void setOperand(string operand);

    private:
        string lable="", operation="", operand="", comment="";
};

#endif // SOURCELINE_H
