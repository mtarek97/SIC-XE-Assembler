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
#include <ValidatorUtilities.h>
#include "ObjectCodeGenerator.h"
#include "OpCodeTable.h"
const std::string ObjectCodeGenerator::SOME_THING_WRONG = "WRONG";
const int ObjectCodeGenerator::operationBitsCount = 8;
const int ObjectCodeGenerator::address4ByteBits = 20;
const int ObjectCodeGenerator::address3ByteBits = 12;
const int ObjectCodeGenerator::flagsBitsCount = 4;
const int ObjectCodeGenerator::min3Byte = -2048;
const int ObjectCodeGenerator::max3Byte = 2047;
const int ObjectCodeGenerator::maxBase = 4095;
const int ObjectCodeGenerator::format3Flag = 300;
const int ObjectCodeGenerator::format4Flag = 400;
const int indirect4Exp = 1000100;
const int indirect4Literal = 1000101;
const int indirect4 = 1000110;
const int indirect3Exp = 1000100;
const int indirect3Literal = 1000100;
const int indirect3 = 1000100;

ObjectCodeGenerator* ObjectCodeGenerator::uniqueInstance = 0;

ObjectCodeGenerator::ObjectCodeGenerator() {
    this->opCodeTable = OpCodeTable::getOpTable();
    this->registersTable = RegistersTable::getARegistersTable();
    this->symbolTable = SymbolTable::getSymbolTable();
    this->expressionEvaluator = ExpressionEvaluator();
    this->literalTable = LiteralTable::getLiteralsTable();
}


ObjectCodeGenerator* ObjectCodeGenerator::getObjectCodeGenerator() {
    if(!uniqueInstance){
        uniqueInstance = new ObjectCodeGenerator();
    }
    return uniqueInstance;
}

void ObjectCodeGenerator::setBaseStatus(bool baseStatus) {
    this->baseStatus = baseStatus;
}

void ObjectCodeGenerator::setCurrentBaseAddress(string currentBaseAddress) {
    this->currentBaseAddress = currentBaseAddress;
}

string ObjectCodeGenerator::getErrorMessage() {
    return this->errorMessage;
}

std::string ObjectCodeGenerator::getObjectCode(SourceLine sourceLine) {
    string operation = sourceLine.getOperation();
    string operand = sourceLine.getOperand();
    //Detect Literal Pool
    if(isLiteral(operation)){
        return getConstantHEX(operation);
    }
    transform(operation.begin(), operation.end(), operation.begin(), ::toupper);
    if(opCodeTable->getInfo(operation).getFormatBytes() == 1){
        return this->opCodeTable->getInfo(operation).getOpCode();
    }
    else if (opCodeTable->getInfo(operation).getFormatBytes() == 2) {
        if (opCodeTable->getInfo(operation).getNumberOfOperands() == 2) {
            string firstRegister = operand.substr(0, operand.find(','));
            string secondRegister = operand.substr(operand.find(',') + 1, operand.size());
            transform(firstRegister.begin(), firstRegister.end(), firstRegister.begin(), ::toupper);
            transform(secondRegister.begin(), secondRegister.end(), secondRegister.begin(), ::toupper);
            return opCodeTable->getInfo(operation).getOpCode() + to_string(registersTable->getRegisterNumber(firstRegister)) + to_string(registersTable->getRegisterNumber(secondRegister));
        }
        else{
            transform(operand.begin(), operand.end(), operand.begin(), ::toupper);
            return opCodeTable->getInfo(operation).getOpCode() +
                   to_string(registersTable->getRegisterNumber(operand)) +
                   "0";
        }
    }
    else{
        if(operation == "RSUB"){
            return calculateObjectCode(opCodeTable->getInfo(operation).getOpCode(),3,0,0,0);
        }
        // Literal Table Needed..
        else if (!isDirective(operation)) {
            string currentCase = getCase(sourceLine);
            string operation = sourceLine.getOperation();
            string operand = sourceLine.getOperand();
            if(currentCase == "CASE0"){ // Format 4 - Indirect (@) - Expression
                string parsed_operation = operation.substr(1,operation.size());
                string parsed_operand = operand.substr(1,operand.size());
                string opCode = opCodeTable->getInfo(parsed_operation).getOpCode();
                int displacement = expressionEvaluator.evaluateExpression(parsed_operand).getLocation();
                return calculateObjectCode(opCode,2,1,displacement,format4Flag);
            }else if(currentCase == "CASE1"){ //Format 4 - Indirect (@)
                string parsed_operation = operation.substr(1,operation.size());
                string parsed_operand = operand.substr(1,operand.size());
                string opCode = opCodeTable->getInfo(parsed_operation).getOpCode();
                int displacement = symbolTable->search(parsed_operand).getLocation();
                return calculateObjectCode(opCode,2,1,displacement,format4Flag);
            }else if(currentCase == "CASE2"){ //Format 3 - Indirect (@) - Absolute Expression
                string parsed_operand = operand.substr(1,operand.size());
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement = expressionEvaluator.evaluateExpression(parsed_operand).getLocation();
                return calculateObjectCode(opCode,2,0,displacement,format3Flag);
            }else if(currentCase == "CASE3"){ //Format 3 - Indirect (@) - Relative (PC) Expression
                string parsed_operand = operand.substr(1,operand.size());
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement = expressionEvaluator.evaluateExpression(parsed_operand).getLocation() - sourceLine.getNextInstruction();
                return calculateObjectCode(opCode,2,2,displacement,format3Flag);
            }else if(currentCase == "CASE4"){ //Format 3 - Indirect (@) - Relative (BASE) Expression
                string parsed_operand = operand.substr(1,operand.size());
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement = expressionEvaluator.evaluateExpression(parsed_operand).getLocation() - symbolTable->search(currentBaseAddress).getLocation();
                return calculateObjectCode(opCode,2,4,displacement,format3Flag);
            }else if(currentCase == "CASE5"){ //Format 3 - Indirect (@) - PC
                string parsed_operand = operand.substr(1,operand.size());
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement =symbolTable->search(parsed_operand).getLocation() - sourceLine.getNextInstruction();
                return calculateObjectCode(opCode,2,2,displacement,format3Flag);
            }else if(currentCase == "CASE6"){ //Format 3 - Indirect (@) - BASE
                string parsed_operand = operand.substr(1,operand.size());
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement =symbolTable->search(parsed_operand).getLocation() - symbolTable->search(currentBaseAddress).getLocation();
                return calculateObjectCode(opCode,2,4,displacement,format3Flag);
            }else if(currentCase == "CASE7"){ //Format 4 - Immediate (#)- Expression
                string parsed_operation = operation.substr(1,operation.size());
                string parsed_operand = operand.substr(1,operand.size());
                string opCode = opCodeTable->getInfo(parsed_operation).getOpCode();
                int displacement = expressionEvaluator.evaluateExpression(parsed_operand).getLocation();
                return calculateObjectCode(opCode,1,1,displacement,format4Flag);
            }else if(currentCase == "CASE8"){ //Format 4 - Immediate (#)- Symbol
                string parsed_operation = operation.substr(1,operation.size());
                string parsed_operand = operand.substr(1,operand.size());
                string opCode = opCodeTable->getInfo(parsed_operation).getOpCode();
                int displacement = symbolTable->search(parsed_operand).getLocation();
                return calculateObjectCode(opCode,1,1,displacement,format4Flag);
            }else if(currentCase == "CASE9"){ //Format 4 - Immediate (#)- Const.
                string parsed_operation = operation.substr(1,operation.size());
                string parsed_operand = operand.substr(1,operand.size());
                string opCode = opCodeTable->getInfo(parsed_operation).getOpCode();
                int displacement = stoi(parsed_operand);
                return calculateObjectCode(opCode,1,1,displacement,format4Flag);
            }else if(currentCase == "CASE10"){ //Format 3 - Immediate (#) - Absolute Expression
                string parsed_operand = operand.substr(1,operand.size());
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement = expressionEvaluator.evaluateExpression(parsed_operand).getLocation();
                return calculateObjectCode(opCode,1,0,displacement,format3Flag);
            }else if(currentCase == "CASE11"){ //Format 3 - Immediate (#) - Relative (PC) Expression
                string parsed_operand = operand.substr(1,operand.size());
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement = expressionEvaluator.evaluateExpression(parsed_operand).getLocation() - sourceLine.getNextInstruction();
                return calculateObjectCode(opCode,1,2,displacement,format3Flag);
            }else if(currentCase == "CASE12"){ //Format 3 - Immediate (#) - Relative (BASE) Expression
                string parsed_operand = operand.substr(1,operand.size());
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement = expressionEvaluator.evaluateExpression(parsed_operand).getLocation() - symbolTable->search(currentBaseAddress).getLocation();
                return calculateObjectCode(opCode,1,4,displacement,format3Flag);
            }else if(currentCase == "CASE13"){ //Format 3 - Immediate (#) - PC - Symbol
                string parsed_operand = operand.substr(1,operand.size());
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement =symbolTable->search(parsed_operand).getLocation() - sourceLine.getNextInstruction();
                return calculateObjectCode(opCode,1,2,displacement,format3Flag);
            }else if(currentCase == "CASE14"){ //Format 3 - Immediate (#) - BASE - Symbol
                string parsed_operand = operand.substr(1,operand.size());
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement =symbolTable->search(parsed_operand).getLocation() - symbolTable->search(currentBaseAddress).getLocation();
                return calculateObjectCode(opCode,1,4,displacement,format3Flag);
            }else if(currentCase == "CASE15"){ //Format 3 - Immediate (#) - Const.
                string parsed_operand = operand.substr(1,operand.size());
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement = stoi(parsed_operand);
                return calculateObjectCode(opCode,1,0,displacement,format3Flag);
            }else if(currentCase == "CASE16"){ // Format 4 - Indexed (,X) - Expression
                string parsed_operation = operation.substr(1,operation.size());
                string parsed_operand = operand.substr(0, operand.find(','));
                string opCode = opCodeTable->getInfo(parsed_operation).getOpCode();
                int displacement = expressionEvaluator.evaluateExpression(parsed_operand).getLocation();
                return calculateObjectCode(opCode,3,9,displacement,format4Flag);
            }else if(currentCase == "CASE17"){ // Format 4 - Indexed (,X)
                string parsed_operation = operation.substr(1,operation.size());
                string parsed_operand = operand.substr(0, operand.find(','));
                string opCode = opCodeTable->getInfo(parsed_operation).getOpCode();
                int displacement = symbolTable->search(parsed_operand).getLocation();
                return calculateObjectCode(opCode,3,9,displacement,format4Flag);
            }else if(currentCase == "CASE18"){ // Format 3 - Indexed (,X) - Absolute Expression
                string parsed_operand = operand.substr(0, operand.find(','));
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement = expressionEvaluator.evaluateExpression(parsed_operand).getLocation();
                return calculateObjectCode(opCode,3,8,displacement,format3Flag);
            }else if(currentCase == "CASE19"){ // Format 3 - Indexed (,X) - Relative (PC) Expression
                string parsed_operand = operand.substr(0, operand.find(','));
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement = expressionEvaluator.evaluateExpression(parsed_operand).getLocation() - sourceLine.getNextInstruction();
                return calculateObjectCode(opCode,3,10,displacement,format3Flag);
            }else if(currentCase == "CASE20"){ // Format 3 - Indexed (,X) - Relative (BASE) Expression
                string parsed_operand = operand.substr(0, operand.find(','));
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement = expressionEvaluator.evaluateExpression(parsed_operand).getLocation() - symbolTable->search(currentBaseAddress).getLocation();
                return calculateObjectCode(opCode,3,12,displacement,format3Flag);
            }else if(currentCase == "CASE21"){ // Format 3 - Indexed (,X) - PC
                string parsed_operand = operand.substr(0, operand.find(','));
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement =symbolTable->search(parsed_operand).getLocation() - sourceLine.getNextInstruction();
                return calculateObjectCode(opCode,3,10,displacement,format3Flag);
            }else if(currentCase == "CASE22"){ // Format 3 - Indexed (,X) - BASE
                string parsed_operand = operand.substr(0, operand.find(','));
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement =symbolTable->search(parsed_operand).getLocation() - symbolTable->search(currentBaseAddress).getLocation();
                return calculateObjectCode(opCode,3,12,displacement,format3Flag);
            }else if(currentCase == "CASE23"){ //Format 4 - Expression
                string parsed_operation = operation.substr(1,operation.size());
                string opCode = opCodeTable->getInfo(parsed_operation).getOpCode();
                int displacement = expressionEvaluator.evaluateExpression(operand).getLocation();
                return calculateObjectCode(opCode,3,1,displacement,format4Flag);
            }else if(currentCase == "CASE24"){ // Format 4 - Literal
                string parsed_operation = operation.substr(1,operation.size());
                string opCode = opCodeTable->getInfo(parsed_operation).getOpCode();
                int displacement = literalTable->getTable()[operand].second;
                return calculateObjectCode(opCode,3,1,displacement,format4Flag);
            }else if(currentCase == "CASE25"){ // Format 4
                string parsed_operation = operation.substr(1,operation.size());
                string opCode = opCodeTable->getInfo(parsed_operation).getOpCode();
                int displacement = symbolTable->search(operand).getLocation();
                return calculateObjectCode(opCode,3,1,displacement,format4Flag);
            }else if(currentCase == "CASE26"){ // Format 3 - Expression - Absolute
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement = expressionEvaluator.evaluateExpression(operand).getLocation();
                return calculateObjectCode(opCode,3,0,displacement,format3Flag);
            }else if(currentCase == "CASE27"){ // Format 3 - Expression - Relative (PC)
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement = expressionEvaluator.evaluateExpression(operand).getLocation() - sourceLine.getNextInstruction();
                return calculateObjectCode(opCode,3,2,displacement,format3Flag);
            }else if(currentCase == "CASE28"){ // Format 3 - Expression - Relative (BASE)
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement = expressionEvaluator.evaluateExpression(operand).getLocation() - symbolTable->search(currentBaseAddress).getLocation();
                return calculateObjectCode(opCode,3,4,displacement,format3Flag);
            }else if(currentCase == "CASE29"){ // Format 3 - Literal - PC
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement = literalTable->getTable()[operand].second - sourceLine.getNextInstruction();
                return calculateObjectCode(opCode,3,2,displacement,format4Flag);
            }else if(currentCase == "CASE30"){ // Format 3 - Literal - BASE
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement = literalTable->getTable()[operand].second - symbolTable->search(currentBaseAddress).getLocation();
                return calculateObjectCode(opCode,3,4,displacement,format3Flag);
            }else if(currentCase == "CASE31"){ // Format 3 - PC
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement =symbolTable->search(operand).getLocation() - sourceLine.getNextInstruction();
                return calculateObjectCode(opCode,3,2,displacement,format3Flag);
            }else if(currentCase == "CASE32"){ // Format 3 - BASE
                string opCode = opCodeTable->getInfo(operation).getOpCode();
                int displacement =symbolTable->search(operand).getLocation() - symbolTable->search(currentBaseAddress).getLocation();
                return calculateObjectCode(opCode,3,4,displacement,format3Flag);
            }else if(currentCase == "CASE_EXPRESSION_ERROR"){ // Expression Evaluator Returned Error !
                sourceLine.setErrorMessage("Error in Expression!");
                return SOME_THING_WRONG;
            }else if(currentCase == "CASE_DISP_ERROR"){ // Displacement is > 2047 || < -2048 in case PC , or < 0 || > 4095 in case BASE !
                sourceLine.setErrorMessage("Displacement Error!");
                return SOME_THING_WRONG;
            }
        }
        else if (operation == "BYTE"){
            return getConstantHEX(operand);
        }
        else if (operation == "WORD"){
            int theWord = stoi(operand);
            std::stringstream stream;
            stream << std::hex << theWord;
            string objectCodeHex = stream.str();
            if (objectCodeHex.size() == 6)
            return objectCodeHex;
            else if (objectCodeHex.size() > 6){
                sourceLine.setErrorMessage("Error in WORD!");
                return SOME_THING_WRONG;
            }
            else{
                while(objectCodeHex.size() < 6){
                    objectCodeHex = "0" + objectCodeHex;
                }
                return objectCodeHex;
            }
        }
        else{
            sourceLine.setErrorMessage("NOT SUPPORTED !!");
            return SOME_THING_WRONG;
        }
    }
}

bool ObjectCodeGenerator::isFormat4Byte(string operation) {
    if(operation.at(0) == '+')
        return true;
    return false;
}

bool ObjectCodeGenerator::isIndexed(string operand) {
    int comma_count = std::count(operand.begin(),operand.end(),',');
    int comma_index = operand.find(',');
    if (comma_count == 1 && (operand.at(comma_index+1 == 'X') ||operand.at(comma_index+1) == 'x'))
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
    if (bits == format3Flag) { // format 3
        bitset<address3ByteBits> addressBits(displacement);
        bitset<address3ByteBits + flagsBitsCount> nonOperation(flags.to_ullong() << addressBits.size() | addressBits.to_ullong());
        bitset<address3ByteBits+ flagsBitsCount + operationBitsCount> objectCode(operationBits.to_ullong() << nonOperation.size() | nonOperation.to_ullong());
        std::stringstream stream;
        stream << std::hex << objectCode.to_ullong();
        objectCodeHex = stream.str();
        while(objectCodeHex.size() < (address3ByteBits+ flagsBitsCount + operationBitsCount) / 4){
            objectCodeHex = "0" + objectCodeHex;
        }
    }
    else{ // format 4
        bitset<address4ByteBits> addressBits(displacement);
        bitset<address4ByteBits + flagsBitsCount> nonOperation(flags.to_ullong() << addressBits.size() | addressBits.to_ullong());
        bitset<address4ByteBits + flagsBitsCount+  operationBitsCount> objectCode(operationBits.to_ullong() << nonOperation.size() | nonOperation.to_ullong());
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

bool ObjectCodeGenerator::isLiteral(string operand) {
    if(operand[0] == '=')
        return true;
    return false;
}

string ObjectCodeGenerator::getConstantHEX(string constant) {
    if(constant[0] == 'C' ||  constant[0] == 'c'){
        string characters = constant.substr(constant.find_first_of('\'')+1,constant.find_last_of('\'')- constant.find_first_of('\'') - 1);
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
       return constant.substr(constant.find_first_of('\'')+1,constant.find_last_of('\'')- constant.find_first_of('\'') - 1);
    };
}
string ObjectCodeGenerator::getCase(SourceLine sourceLine){
    string operation = sourceLine.getOperation();
    string operand = sourceLine.getOperand();
    if(isIndirect(operand)){
        string parsed_operand = operand.substr(1,operand.size());
        if(isFormat4Byte(operation)){
            if(ValidatorUtilities::isExpression(parsed_operand,false)){
                if(expressionEvaluator.evaluateExpression(parsed_operand).getType()!='e')
                    return "CASE0";
                return "CASE_EXPRESSION_ERROR";
            }
            else{
                return "CASE1";
            }
        }
        else{ // Format 3
            if(ValidatorUtilities::isExpression(parsed_operand,false)){
                SymbolInfo info = expressionEvaluator.evaluateExpression(parsed_operand);
                if(info.getType() == 'a'){
                    if(info.getLocation() > max3Byte || info.getLocation() < min3Byte)
                        return "CASE_DISP_ERROR";
                    return "CASE2";
                }
                else if(info.getType() == 'r'){
                    int displacement = info.getLocation() - sourceLine.getNextInstruction();
                    if(displacement >= min3Byte && displacement <= max3Byte){
                        return "CASE3";
                    }
                    else{
                        if(baseStatus) {
                            int base_disp = info.getLocation() - symbolTable->search(currentBaseAddress).getLocation();
                            if ( base_disp >= 0 && base_disp <= maxBase)
                                return "CASE4";
                        }else{
                            return "CASE_DISP_ERROR";
                        }
                    }
                }
                else{
                    return "CASE_EXPRESSION_ERROR";
                }
            }
            else{
                int displacement = symbolTable->search(parsed_operand).getLocation() - sourceLine.getNextInstruction();
                if(displacement >= min3Byte && displacement <= max3Byte){ // PC
                    return "CASE5";
                }
                else{
                    if(baseStatus) {
                        int base_disp = symbolTable->search(parsed_operand).getLocation() - symbolTable->search(currentBaseAddress).getLocation();
                        if ( base_disp >= 0 && base_disp <= maxBase)
                            return "CASE6";
                    }else{
                        return "CASE_DISP_ERROR";
                    }
                }
            }
        }
    }
    else if(isImmediate(operand)){
        string parsed_operand = operand.substr(1,operand.size());
        if(isFormat4Byte(operation)){
            if(ValidatorUtilities::isExpression(parsed_operand,false)){
                if(expressionEvaluator.evaluateExpression(parsed_operand).getType()!='e')
                    return "CASE7";
                return "CASE_EXPRESSION_ERROR";
            }
            else if(symbolTable->search(parsed_operand).getLocation() != -1)
                return "CASE8";
            else{
                return "CASE9";
            }
        }
        else { // Format 3
            if(ValidatorUtilities::isExpression(parsed_operand,false)){
                SymbolInfo info = expressionEvaluator.evaluateExpression(parsed_operand);
                if(info.getType() == 'a'){
                    if(info.getLocation() > max3Byte || info.getLocation() < min3Byte)
                        return "CASE_DISP_ERROR";
                    return "CASE10";
                }
                else if(info.getType() == 'r'){
                    int displacement = info.getLocation() - sourceLine.getNextInstruction();
                    if(displacement >= min3Byte && displacement <= max3Byte){
                        return "CASE11";
                    }
                    else{
                        if(baseStatus) {
                            int base_disp = info.getLocation() - symbolTable->search(currentBaseAddress).getLocation();
                            if ( base_disp >= 0 && base_disp <= maxBase)
                                return "CASE12";
                        }else{
                            return "CASE_DISP_ERROR";
                        }
                    }
                }
                else{
                    return "CASE_EXPRESSION_ERROR";
                }
            }
            else if (symbolTable->search(parsed_operand).getLocation() != -1) {
                int displacement = symbolTable->search(parsed_operand).getLocation() - sourceLine.getNextInstruction();
                if (displacement >= min3Byte && displacement <= max3Byte) { // PC
                    return "CASE13";
                } else {
                    if (baseStatus) {
                        int base_disp = symbolTable->search(parsed_operand).getLocation() - symbolTable->search(currentBaseAddress).getLocation();
                        if (base_disp >= 0 && base_disp <= maxBase)
                            return "CASE14";
                    } else {
                        return "CASE_DISP_ERROR";
                    }
                }
            }
            else{
                return "CASE15";
            }
        }
    }
    else if (isIndexed(operand)){
        string parsed_operand = operand.substr(0, operand.find(','));
        if(isFormat4Byte(operation)){
            if(ValidatorUtilities::isExpression(parsed_operand,false)){
                if(expressionEvaluator.evaluateExpression(parsed_operand).getType()!='e')
                    return "CASE16";
                return "CASE_EXPRESSION_ERROR";
            }
            else{
                return "CASE17";
            }
        }
        else{ // Format 3
            if(ValidatorUtilities::isExpression(parsed_operand,false)){
                SymbolInfo info = expressionEvaluator.evaluateExpression(parsed_operand);
                if(info.getType() == 'a'){
                    if(info.getLocation() > max3Byte || info.getLocation() < min3Byte)
                        return "CASE_DISP_ERROR";
                    return "CASE18";
                }
                else if(info.getType() == 'r'){
                    int displacement = info.getLocation() - sourceLine.getNextInstruction();
                    if(displacement >= min3Byte && displacement <= max3Byte){
                        return "CASE19";
                    }
                    else{
                        if(baseStatus) {
                            int base_disp = info.getLocation() - symbolTable->search(currentBaseAddress).getLocation();
                            if ( base_disp >= 0 && base_disp <= maxBase)
                                return "CASE20";
                        }else{
                            return "CASE_DISP_ERROR";
                        }
                    }
                }
                else{
                    return "CASE_EXPRESSION_ERROR";
                }
            }
            else{
                int displacement = symbolTable->search(parsed_operand).getLocation() - sourceLine.getNextInstruction();
                if(displacement >= min3Byte && displacement <= max3Byte){ // PC
                    return "CASE21";
                }
                else{
                    if(baseStatus) {
                        int base_disp = symbolTable->search(parsed_operand).getLocation() - symbolTable->search(currentBaseAddress).getLocation();
                        if ( base_disp >= 0 && base_disp <= maxBase)
                            return "CASE22";
                    }else{
                        return "CASE_DISP_ERROR";
                    }
                }
            }
        }
    }
    else{
        if(isFormat4Byte(operation)){
            if(ValidatorUtilities::isExpression(operand,false)){
                if(expressionEvaluator.evaluateExpression(operand).getType()!='e')
                    return "CASE23";
                return "CASE_EXPRESSION_ERROR";
            }
            else if (isLiteral(operand)){
                return "CASE24";
            }
            else{
                return "CASE25";
            }
        }
        else{ // Format 3
            if(ValidatorUtilities::isExpression(operand,false)){
                SymbolInfo info = expressionEvaluator.evaluateExpression(operand);
                if(info.getType() == 'a'){
                    if(info.getLocation() > max3Byte || info.getLocation() < min3Byte)
                        return "CASE_DISP_ERROR";
                    return "CASE26";
                }
                else if(info.getType() == 'r'){
                    int displacement = info.getLocation() - sourceLine.getNextInstruction();
                    if(displacement >= min3Byte && displacement <= max3Byte){
                        return "CASE27";
                    }
                    else{
                        if(baseStatus) {
                            int base_disp = info.getLocation() - symbolTable->search(currentBaseAddress).getLocation();
                            if ( base_disp >= 0 && base_disp <= maxBase)
                                return "CASE28";
                        }else{
                            return "CASE_DISP_ERROR";
                        }
                    }
                }
                else{
                    return "CASE_EXPRESSION_ERROR";
                }
            }
            else if (isLiteral(operand)){
                int displacement = literalTable->getTable()[operand].second - sourceLine.getNextInstruction();
                if(displacement >= min3Byte && displacement <= max3Byte){ // PC
                    return "CASE29";
                }
                else{
                    if(baseStatus) {
                        int base_disp = literalTable->getTable()[operand].second - symbolTable->search(currentBaseAddress).getLocation();
                        if ( base_disp >= 0 && base_disp <= maxBase)
                            return "CASE30";
                    }else{
                        return "CASE_DISP_ERROR";
                    }
                }
            }
            else{
                int displacement = symbolTable->search(operand).getLocation() - sourceLine.getNextInstruction();
                if(displacement >= min3Byte && displacement <= max3Byte){ // PC
                    return "CASE31";
                }
                else{
                    if(baseStatus) {
                        int base_disp = symbolTable->search(operand).getLocation() - symbolTable->search(currentBaseAddress).getLocation();
                        if ( base_disp >= 0 && base_disp <= maxBase)
                            return "CASE32";
                    }else{
                        return "CASE_DISP_ERROR";
                    }
                }
            }
        }
    }
}




