#include "OpInfo.h"
#include "OpCodeTable.h"
#include "SourceLine.h"
#include <string.h>
#include <vector>
#include <string>
#include <algorithm>
#include <ctype.h>
#include <sstream>
#include <regex>

#ifndef VALIDATORUTILITIES_H_INCLUDED
#define VALIDATORUTILITIES_H_INCLUDED


class ValidatorUtilities
{
    public:
        static bool isSymbol(std::string str, int maxLength);
        static bool isHexAddress(std::string str, int maxlength);
        static bool isDecimalNumber(std::string str, int maxDigitsCount, bool canBeNegative);
        static bool isReservedKeyword(std::string str);
        static bool isDirective(std::string str);
        static std::vector<std::string> split(std::string str, string regexString);
        static SourceLine* toUpperCase(SourceLine* srcLine);
        static bool isExpression(std::string str, bool canBeGeneralExpression);
        static bool isLiteral(std::string str);

    private:
        ValidatorUtilities();
        static OpCodeTable* opCodeTable;
        static const std::string directives[];
};


#endif // VALIDATORUTILITIES_H_INCLUDED
