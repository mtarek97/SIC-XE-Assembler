#include "OpInfo.h"
#include "OpCodeTable.h"
#include "SourceLine.h"
#include <string.h>
#include <vector>
#include <string>
#include "ValidatorUtilities.h"
#include "RegistersTable.h"


#ifndef SYNTAXVALIDATOR_H_INCLUDED
#define SYNTAXVALIDATOR_H_INCLUDED

class SyntaxValidator
{
    public:
        SyntaxValidator();
        bool isValid(SourceLine* srcLine);
        string getErrorMessage();

    private:
        static const int INVALID;
        static const int INSTRUCTION;
        static const int DIRECTIVE;
        static const int LABEL_MAXLENGTH;
        OpCodeTable* opCodeTable;
        RegistersTable* registers;
        SourceLine* sourceLine;
        std::string errorMessage;
        int checkOperation(std::string operation);
        vector<std::string> split(string str, char delimiter);
        bool checkDirectiveOperand();
        bool checkFormat2Operand(OpInfo info);
        bool checkFormat3or4Operand(OpInfo info);
};

#endif // VALIDATOR_H_INCLUDED
