#include "OpInfo.h"
#include "OpCodeTable.h"
#include "SymbolTable.h"
#include "SourceLine.h"
#include <sstream>
#include <string.h>
#include <vector>
#include <string>
#include <algorithm>
#include <ctype.h>

#ifndef SYNTAXVALIDATOR_H_INCLUDED
#define SYNTAXVALIDATOR_H_INCLUDED

class SyntaxValidator
{
    public:
        SyntaxValidator();
        bool isValid(SourceLine srcLine);
        string getErrorMessage();
        static const int INVALID;
        static const int INSTRUCTION;
        static const int DIRECTIVE;


    private:
        OpCodeTable* opCodeTable;
        SourceLine sourceLine;
        std::string errorMessage;
        static const std::string registers[];
        int checkOperation(std::string operation);
        vector<std::string> split(string str, char delimiter);
        bool checkDirectiveOperand();
        bool checkFormat2Operand(OpInfo info);
        bool checkFormat3or4Operand(OpInfo info);
        bool isSymbol(string str, int maxLength);
        bool isHexAddress(string str, int maxlength);
        bool isReservedKeyword(string str);
        bool isRegister(string str);
};

#endif // VALIDATOR_H_INCLUDED
