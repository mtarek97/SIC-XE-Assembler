//
// Created by Mohamed Mashaal on 6/4/2018.
//
/*
        ObjectCodeGenerator Guide :
        ObjectCodeGenerator* generator = ObjectCodeGenerator::getObjectCodeGenerator(); getting instance
        generator->setSymbolTable(symbolTable); you must set the symbol table after finishing pass one so that the generator can calculate displacement
        string result = generator->getObjectCode(sourceLine); returns the objectCode representing this Instruction. SourceLine Object (NextInstruction member variable)
        should be filled as it is needed in the calculation of the displacement.
        Next Instruction is set in int not HEX (same as location counter being kept in code currently).
        Almost no validation is being done, so don't pass for example RESW, REWSB or EQU those Directives have no objectcode !
        Literals is missing, since no literal locations is found since there is no literal handling yet!
        Also EQU should be added may be to symbol table because of the displacement calculations.
    */
#include "ObjectCodeGenerator.h"
#include "OpCodeTable.h"
const std::string ObjectCodeGenerator::SOME_THING_WRONG = "WRONG";
const int ObjectCodeGenerator::operationBitsCount = 8;
const int ObjectCodeGenerator::address4ByteBits = 20;
const int ObjectCodeGenerator::address3ByteBits = 12;
const int ObjectCodeGenerator::flagsBitsCount = 4;



ObjectCodeGenerator* ObjectCodeGenerator::uniqueInstance = 0;

ObjectCodeGenerator::ObjectCodeGenerator() {
    this->opCodeTable = OpCodeTable::getOpTable();
    this->registersTable = RegistersTable::getARegistersTable();
}

void ObjectCodeGenerator::setSymbolTable(SymbolTable symbolTable) {
    this->symbolTable = symbolTable;
}

ObjectCodeGenerator* ObjectCodeGenerator::getObjectCodeGenerator() {
    if(!uniqueInstance){
        uniqueInstance = new ObjectCodeGenerator();
    }
    return uniqueInstance;
}

std::string ObjectCodeGenerator::getObjectCode(SourceLine sourceLine) {
    string operation = sourceLine.getOperation();
    transform(operation.begin(), operation.end(), operation.begin(), ::toupper);
    if(opCodeTable->getInfo(operation).getFormatBytes() == 1){
        return this->opCodeTable->getInfo(operation).getOpCode();
    }
    else if (opCodeTable->getInfo(operation).getFormatBytes() == 2) {
        if (opCodeTable->getInfo(operation).getNumberOfOperands() == 2) {
            string operand = sourceLine.getOperand();
            string firstRegister = operand.substr(0, operand.find(','));
            string secondRegister = operand.substr(operand.find(',') + 1, operand.size());
            transform(firstRegister.begin(), firstRegister.end(), firstRegister.begin(), ::toupper);
            transform(secondRegister.begin(), secondRegister.end(), secondRegister.begin(), ::toupper);
            return opCodeTable->getInfo(operation).getOpCode() +
                   to_string(registersTable->getRegisterNumber(firstRegister)) +
                   to_string(registersTable->getRegisterNumber(secondRegister));
        }
        else{
            string operand = sourceLine.getOperand();
            transform(operand.begin(), operand.end(), operand.begin(), ::toupper);
            return opCodeTable->getInfo(operation).getOpCode() +
                   to_string(registersTable->getRegisterNumber(operand)) +
                   "0";
        }
    }
    else{
        string operand = sourceLine.getOperand();
        if(operation == "RSUB"){
            return calculateObjectCode(opCodeTable->getInfo(operation).getOpCode(),3,0,0,0);
        }
        else if (!isDirective(operation)) {
            if (!isFormat4Byte(operation) && !isIndexed(operand) && !isImmediate(operand) &&
                     !isIndirect(operand)) {
                int displacement = symbolTable.search(operand) - sourceLine.getNextInstruction();
                return calculateObjectCode(opCodeTable->getInfo(operation).getOpCode(), 3, 2, displacement, 0);
            } else if (isFormat4Byte(operation) && !isIndexed(operand) && !isImmediate(operand) &&
                       !isIndirect(operand)) {
                int address = symbolTable.search(operand);
                return calculateObjectCode(opCodeTable->getInfo(operation.substr(1, operation.size())).getOpCode(), 3,
                                           1, address, 1);
            } else if (!isFormat4Byte(operation) && isIndexed(operand) && !isImmediate(operand) &&
                       !isIndirect(operand)) {
                int displacement =
                        symbolTable.search(operand.substr(0, operand.find(','))) - sourceLine.getNextInstruction();
                return calculateObjectCode(opCodeTable->getInfo(operation).getOpCode(), 3, 6, displacement, 0);
            } else if (isFormat4Byte(operation) && isIndexed(operand) && !isImmediate(operand) &&
                       !isIndirect(operand)) {
                int displacement = symbolTable.search(operand.substr(0, operand.find(',')));
                return calculateObjectCode(opCodeTable->getInfo(operation.substr(1, operation.size())).getOpCode(), 3,
                                           5, displacement, 1);
            } else if (!isFormat4Byte(operation) && !isIndexed(operand) && !isImmediate(operand) &&
                       isIndirect(operand)) {
                int displacement =
                        symbolTable.search(operand.substr(1, operand.size())) - sourceLine.getNextInstruction();
                return calculateObjectCode(opCodeTable->getInfo(operation).getOpCode(), 2, 2, displacement, 0);
            } else if (isFormat4Byte(operation) && !isIndexed(operand) && !isImmediate(operand) &&
                       isIndirect(operand)) {
                int displacement = symbolTable.search(operand.substr(1, operand.size()));
                return calculateObjectCode(opCodeTable->getInfo(operation.substr(1, operation.size())).getOpCode(), 2,
                                           1, displacement, 1);
            } else if (!isFormat4Byte(operation) && !isIndexed(operand) && isImmediate(operand) &&
                       !isIndirect(operand)) {
                if (symbolTable.search(operand.substr(1, operand.size())) != -1) {
                    int displacement =
                            symbolTable.search(operand.substr(1, operand.size())) - sourceLine.getNextInstruction();
                    return calculateObjectCode(opCodeTable->getInfo(operation).getOpCode(), 1, 2, displacement, 0);
                } else {
                    int displacement = stoi(operand.substr(1, operand.size()));
                    return calculateObjectCode(opCodeTable->getInfo(operation).getOpCode(), 1, 0, displacement, 0);
                }

            } else if (isFormat4Byte(operation) && !isIndexed(operand) && isImmediate(operand) &&
                       !isIndirect(operand)) {
                if (symbolTable.search(operand.substr(1, operand.size())) != -1) {
                    int displacement = symbolTable.search(operand.substr(1, operand.size()));
                    return calculateObjectCode(opCodeTable->getInfo(operation.substr(1, operation.size())).getOpCode(), 1,
                                               1, displacement, 1);
                } else {
                    int displacement = stoi(operand.substr(1, operand.size()));
                    return calculateObjectCode(opCodeTable->getInfo(operation.substr(1, operation.size())).getOpCode(), 1, 1, displacement, 1);
                }

            }
        }
        else if (operation == "BYTE"){
            if(operand[0] == 'C' ||  operand[0] == 'c'){
                string characters = operand.substr(operand.find_first_of('\'')+1,operand.find_last_of('\'')- operand.find_first_of('\'') - 1);
                string result = "";
                for(char character : characters){
                    std::stringstream stream;
                    stream << std::hex << int(character);
                    string objectCodeHex = stream.str();
                    result += objectCodeHex;
                }
                return result;
            }
            else{
                return operand.substr(operand.find_first_of('\'')+1,operand.find_last_of('\'')- operand.find_first_of('\'') - 1);
            }
        }
        else if (operation == "WORD"){
            int theWord = stoi(operand);
            std::stringstream stream;
            stream << std::hex << theWord;
            string objectCodeHex = stream.str();
            return objectCodeHex;
        }
        else{
            return SOME_THING_WRONG;
        }
        //TODO: literals object code to be added.
    }
}

bool ObjectCodeGenerator::isFormat4Byte(string operation) {
    if(operation.at(0) == '+')
        return true;
    return false;
}

bool ObjectCodeGenerator::isIndexed(string operand) {
    if (operand.find(',') != -1)
        return true;
    return false;
}

bool ObjectCodeGenerator::isImmediate(string operand) {
    if(operand.at(0) == '#')
        return true;
    return false;
}

string ObjectCodeGenerator::calculateObjectCode(string operationCode, int ni, int xbpe,
                                                int displacement,int bits) {
    long long operation;
    istringstream ost(operationCode);
    ost >> hex >> operation;
    operation += ni;
    bitset<operationBitsCount> operationBits(operation);
    bitset<flagsBitsCount> flags(xbpe);
    string objectCodeHex = "";
    if (bits == 0) {
        bitset<address3ByteBits> addressBits(displacement);
        bitset<address3ByteBits + flagsBitsCount> nonOperation(
                flags.to_ullong() << addressBits.size() | addressBits.to_ullong());
        bitset<address3ByteBits+ flagsBitsCount + operationBitsCount> objectCode(
                operationBits.to_ullong() << nonOperation.size() | nonOperation.to_ullong());
        std::stringstream stream;
        stream << std::hex << objectCode.to_ullong();
        objectCodeHex = stream.str();
        while(objectCodeHex.size() < (address3ByteBits+ flagsBitsCount + operationBitsCount) / 4){
            objectCodeHex = "0" + objectCodeHex;
        }
    }
    else{
        bitset<address4ByteBits> addressBits(displacement);
        bitset<address4ByteBits + flagsBitsCount> nonOperation(
                flags.to_ullong() << addressBits.size() | addressBits.to_ullong());
        bitset<address4ByteBits + flagsBitsCount+  operationBitsCount> objectCode(
                operationBits.to_ullong() << nonOperation.size() | nonOperation.to_ullong());
        std::stringstream stream;
        stream << std::hex << objectCode.to_ullong();
        objectCodeHex = stream.str();
        while(objectCodeHex.size() < (address4ByteBits+ flagsBitsCount + operationBitsCount) / 4){
            objectCodeHex = "0" + objectCodeHex;
        }
    }
    return objectCodeHex;
}

bool ObjectCodeGenerator::isIndirect(string operand) {
    if(operand.at(0) == '@')
        return true;
    return false;
}

bool ObjectCodeGenerator::isDirective(string operation) {
    if (operation == "BYTE" || operation == "WORD")
        return true;
    return false;
}


