//
// Created by Mohamed Mashaal on 6/4/2018.
//

#ifndef SIC_XE_ASSEMBLER_OBJECTCODEGENERATOR_H
#define SIC_XE_ASSEMBLER_OBJECTCODEGENERATOR_H

#include <string>
#include "SourceLine.h"
#include "OpCodeTable.h"
#include "SymbolTable.h"
#include "RegistersTable.h"
#include "ExpressionEvaluator.h"
#include "LiteralTable.h"

class ObjectCodeGenerator{
    public:
    static ObjectCodeGenerator* getObjectCodeGenerator();
    std::string getObjectCode(SourceLine* sourceLine);
    static const std::string SOME_THING_WRONG;
    void setBaseStatus(bool baseStatus);
    void setCurrentBaseAddress(string currentBaseAddress);
    string getErrorMessage();
    private:
    static ObjectCodeGenerator* uniqueInstance;
    ObjectCodeGenerator();
    OpCodeTable* opCodeTable;
    RegistersTable* registersTable;
    SymbolTable* symbolTable;
    ExpressionEvaluator expressionEvaluator;
    LiteralTable* literalTable;
    bool baseStatus;
    string errorMessage;
    string currentBaseAddress;
    bool isFormat4Byte(string basic_string);
    bool isIndexed(string basic_string);
    bool isImmediate(string basic_string);
    static const int operationBitsCount ;
    static const int address4ByteBits;
    static const int address3ByteBits;
    static const int flagsBitsCount;
    static const int min3Byte;
    static const int max3Byte;
    static const int maxBase;
    static const int format3Flag;
    static const int format4Flag;
    string calculateObjectCode(string operationCode, int ni, int xbpe, int displacement, int byteFormat);
    string getCase(SourceLine* sourceLine);
    bool isIndirect(string basic_string);

    bool isDirective(string basic_string);

    bool isLiteral(string basic_string);

    string getConstantHEX(string constant);

    string getConstantHEXLiteral(SourceLine* sourceLine, basic_string<char, char_traits<char>, allocator<char>> basic_string);
};



#endif //SIC_XE_ASSEMBLER_OBJECTCODEGENERATOR_H
