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

class ObjectCodeGenerator{
    public:
    static ObjectCodeGenerator* getObjectCodeGenerator();
    std::string getObjectCode(SourceLine sourceLine);
    static const std::string SOME_THING_WRONG;
    void setBaseStatus(bool baseStatus);
    void setCurrentBaseAddress(string currentBaseAddress);
    private:
    static ObjectCodeGenerator* uniqueInstance;
    ObjectCodeGenerator();
    OpCodeTable* opCodeTable;
    RegistersTable* registersTable;
    SymbolTable* symbolTable;
    bool baseStatus;
    string currentBaseAddress;
    bool isFormat4Byte(string basic_string);
    bool isIndexed(string basic_string);
    bool isImmediate(string basic_string);
    static const int operationBitsCount ;
    static const int address4ByteBits;
    static const int address3ByteBits;
    static const int flagsBitsCount;
    string calculateObjectCode(string operationCode, int ni, int xbpe, int displacement, int byteFormat);

    bool isIndirect(string basic_string);

    bool isDirective(string basic_string);

    bool isLiteral(string basic_string);

    string getConstantHEX(string constant);
};



#endif //SIC_XE_ASSEMBLER_OBJECTCODEGENERATOR_H
