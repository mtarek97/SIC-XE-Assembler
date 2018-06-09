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

#ifndef OBJECTPROGRAM_H
#define OBJECTPROGRAM_H


class ObjectProgram
{
    public:
        ObjectProgram();
        ///length of program and startingAddress in HEX.
        void writeHeader(string progName,string startingAddress,string lengthOfProg);
        ///startingAddress in HEX and lengthOFObjCodes in bytes in HEX.
        void writeText(string startingAddress,string lengthOFObjCodes,string objCodes);
        ///executionAddress in HEX.
        void writeEnd(string executionAddress);
        ///startingAddress in HEX. length variable is the length of the address field to be modified in half bytes.
        void writeModification(string startingAddress, string Length);
    protected:

    private:
};

#endif // OBJECTPROGRAM_H
