#include "SyntaxValidator.h"

const int SyntaxValidator::INVALID = 0;
const int SyntaxValidator::INSTRUCTION = 1;
const int SyntaxValidator::DIRECTIVE = 2;
const std::string SyntaxValidator::registers[] = {"A","X","L","B","S","T","F","PC","SW"};

// constructor
SyntaxValidator::SyntaxValidator(){
    this->opCodeTable = OpCodeTable::getOpTable();
}

bool SyntaxValidator::isValid(SourceLine srcLine){
    this->sourceLine = srcLine;
    if(sourceLine.getOperation()=="END"){
        if(sourceLine.getLable()!=""){
            this->errorMessage = "label must be blank at the END statement";
            return false;
        }
    }else{
        if(isReservedKeyword(sourceLine.getLable()) || isRegister(sourceLine.getLable())){
            this->errorMessage = "label can't be a reserved keyword";
            return false;
        }else if(!isSymbol(sourceLine.getLable(),8)){
            this->errorMessage = sourceLine.getLable() + " is invalid label";
            return false;
        }else{
            int operationType = checkOperation(this->sourceLine.getOperation());
            if(operationType == this->INSTRUCTION){
                OpInfo info = opCodeTable->getInfo(this->sourceLine.getOperation());
                if(info.getFormateBytes() == 2){
                    return this->checkFormat2Operand(info);
                }else{  // format 3 or 4
                    return this->checkFormat3or4Operand(info);
                }
            }else if(operationType == this->DIRECTIVE){
                    return this->checkDirectiveOperand();
            }else{  // not valid operation
                return false;
            }
        }
    }
}

string SyntaxValidator::getErrorMessage(){
    return this->errorMessage;
}

int SyntaxValidator::checkOperation(std::string operation){
    // check for format 4 (+op)
    bool prefixedByPlus = false;
    std::string nonPrefixedOperation;
    if(operation[0]=='+'){
        prefixedByPlus = true;
        nonPrefixedOperation = operation.substr(1);
        this->sourceLine.setOperation(nonPrefixedOperation);
    }else{
        nonPrefixedOperation = operation;
    }
    OpInfo info = opCodeTable->getInfo(nonPrefixedOperation);
    if(info.getOpCode() != OpInfo::NOT_FOUND){
        if(info.getFormateBytes() == 2 && prefixedByPlus){
            this->errorMessage = "can't use format 4 with " + nonPrefixedOperation;
            return INVALID;
        }
        return INSTRUCTION;
    }
    ///ToDO: edit directives DS
    else if(operation=="BYTE" || operation =="WORD"
            || operation == "RESB" || operation =="RESW"
            || operation=="START" || operation =="END"){
        return DIRECTIVE;
    }else{
        this->errorMessage = operation + " is invalid operation";
        return INVALID;
    }
}

bool SyntaxValidator::checkFormat2Operand(OpInfo info){
    vector<std::string> splittedOperands = this->split(sourceLine.getOperand(),',');
    int noOfOperands = info.getNumberOfOperands();
    if(splittedOperands.size() == noOfOperands){
        for(int i=0;i<noOfOperands;i++){
            if(!isRegister(splittedOperands[i])){
                errorMessage = splittedOperands[i] + " is invalid register";
                return false;
            }
        }
    }else{
        this->errorMessage = "number of operands must be " + to_string(noOfOperands);
        return false;
    }
    return true;
}

bool SyntaxValidator::checkFormat3or4Operand(OpInfo info){
    if(sourceLine.getOperation() == "RSUB"){
        if(sourceLine.getOperand() != ""){
            this->errorMessage = "RSUB can't take any operand";
            return false;
        }
    }
    vector<std::string> splittedOperands = this->split(sourceLine.getOperand(),',');
    if(splittedOperands.size()==2){
        if(!(splittedOperands[1]=="X")){
            this->errorMessage = "can't use register rather than X to indexed addressing";
            return false;
        }
        // we can show special message for immediate or indirect addressing with indexed (if we have time)
        else if(isRegister(splittedOperands[0])){
            this->errorMessage = "can't use registers with indexed addressing" ;
            return false;
        }
        else if(isReservedKeyword(splittedOperands[0])){
            this->errorMessage = splittedOperands[0] + " is invalid operand because it's a reserved keyword";
            return false;
        }
        else if(!(isSymbol(splittedOperands[0],8)) && !(isHexAddress(splittedOperands[0],6))){
            this->errorMessage = splittedOperands[0] + " is invalid operand";
            return false;
        }
        /// TODO : handle literals
        else{
            return true;
        }
    }else if(splittedOperands.size()==1){
        string nonPrefixedOperand = splittedOperands[0];
        if(splittedOperands[0][0] == '#' || splittedOperands[0][0] == '@'){
                nonPrefixedOperand = splittedOperands[0].substr(1);
        }
        if(isRegister(nonPrefixedOperand)){
            this->errorMessage = nonPrefixedOperand + " is invalid operand because it's a register";
            return false;
        }
        else if(isReservedKeyword(nonPrefixedOperand)){
            this->errorMessage = nonPrefixedOperand + " is invalid operand";
            return false;
        }
        if(!isSymbol(nonPrefixedOperand,8) && !isHexAddress(nonPrefixedOperand,6)){
            this->errorMessage = splittedOperands[0] + " is invalid operand";
            return false;
        }
        /// TODO : handle literals
        else{
            return true;
        }

    }else{
        this->errorMessage = "number of operands must be 1";
        return false;
    }
}

/// TODO: handle directives
bool SyntaxValidator::checkDirectiveOperand(){

    return true;
}

// stack-over-flow edited method :(
vector<string> SyntaxValidator::split(string str, char delimiter) {
    stringstream strStream(str);
    string segment;
    vector<string> tokens;
    while (getline(strStream, segment, delimiter)) {
        tokens.push_back(segment);
    }
    return tokens;
}

bool SyntaxValidator::isSymbol(string str,int maxLength){
    if(str.size() > maxLength){
        return false;
    }
    for(int i=0;i<str.size();i++){
        if(!isalpha(str[i]) && !isdigit(str[i])){
            return false;
        }
    }
    return true;
}

bool SyntaxValidator::isReservedKeyword(string str){
    if(str=="BYTE" || str =="WORD"
            || str == "RESB" || str =="RESW"
            || str=="START" || str =="END"){
                    return true;
            }
    OpInfo info = opCodeTable->getInfo(str);
    if(info.getOpCode() != OpInfo::NOT_FOUND){
        return true;
    }
    return false;
}

bool SyntaxValidator::isHexAddress(string str, int maxLength){
    if(str.size()>maxLength){
        return false;
    }
    for(int i=0;i<str.size();i++){
        if(!isxdigit(str[i])){
            return false;
        }
    }
    return true;
}

bool SyntaxValidator::isRegister(string str){
    if(find(registers,registers+9,str) == registers+9){
        return false;
    }
    return true;
}

