#include<bits/stdc++.h>
#include<SourceProgram.h>
#include<SourceLine.h>
#include<OpCodeTable.h>
#include<SyntaxValidator.h>
#include<AssemblyListing.h>
#include<SymbolTable.h>
#include<ValidatorUtilities.h>
#include<UpdateLocationCounter.h>
#include <ObjectCodeGenerator.h>
#include "ObjectProgram.h"

#ifndef PASS2_H
#define PASS2_H


class Pass2
{
    public:
        Pass2(vector<SourceLine> sourceLinesArr,int lengthOfProg,SymbolTable symbolTable);
        void generateObjProg();
    protected:

    private:
        vector<SourceLine> sourceLinesArr;
        int linesCounter = 0;
        int lengthOfProg;
        ObjectProgram objectProgram;
        ObjectCodeGenerator* objCodeGenerator;
        string TextRecord;
        string TextStartAddress;
        int MAX_TEXT_RECORED_LENGTH = 60;
        SymbolTable symbolTable;
        string convertToHEX(int num);
};

#endif // PASS2_H
