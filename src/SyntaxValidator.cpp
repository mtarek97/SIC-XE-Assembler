#include "SyntaxValidator.h"

const int SyntaxValidator::INVALID = 0;
const int SyntaxValidator::INSTRUCTION = 1;
const int SyntaxValidator::DIRECTIVE = 2;
const int SyntaxValidator::LABEL_MAXLENGTH = 8;

// constructor
SyntaxValidator::SyntaxValidator(){
    this->opCodeTable = OpCodeTable::getOpTable();
}

bool SyntaxValidator::isValid(SourceLine srcLine){
    this->sourceLine = ValidatorUtilities::toUpperCase(srcLine);
    if(sourceLine.getOperation()=="END" && sourceLine.getLable()!=""){
        this->errorMessage = "label must be blank at the END statement";
        return false;
    }else{
        if(ValidatorUtilities::isReservedKeyword(sourceLine.getLable())
            || ValidatorUtilities::isRegister(sourceLine.getLable())){
            this->errorMessage = "label can't be a reserved keyword";
            return false;
        }else if(!ValidatorUtilities::isSymbol(sourceLine.getLable(),LABEL_MAXLENGTH)){
            this->errorMessage = sourceLine.getLable() + " is invalid. label must start with a letter and consist of letters, numbers, or '_'";
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
    else if(ValidatorUtilities::isDirective(operation)){
        return DIRECTIVE;
    }else{
        this->errorMessage = operation + " is invalid operation";
        return INVALID;
    }
}

bool SyntaxValidator::checkFormat2Operand(OpInfo info){
    string operand =  sourceLine.getOperand();
    if(operand[0]==',' || operand[operand.size()-1]==','){
        errorMessage = operand + " is invalid operand";
            return false;
    }
    vector<std::string> splittedOperands = ValidatorUtilities::split(operand,',');
    int noOfOperands = info.getNumberOfOperands();
    if(splittedOperands.size() == noOfOperands){
        for(int i=0;i<noOfOperands;i++){
            if(!ValidatorUtilities::isRegister(splittedOperands[i])){
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
        }else{
            return true;
        }
    }
    string operand = sourceLine.getOperand();
    if(operand[0]==',' || operand[operand.size()-1]==','){
        errorMessage = operand + " is invalid operand";
            return false;
    }
    vector<std::string> splittedOperands = ValidatorUtilities::split(operand,',');
    if(splittedOperands.size()==2){
        if(!(splittedOperands[1]=="X")){
            this->errorMessage = "can't use register rather than X to indexed addressing";
            return false;
        }
        else if(ValidatorUtilities::isRegister(splittedOperands[0])){
            this->errorMessage = "can't use registers with indexed addressing" ;
            return false;
        }
        else if(ValidatorUtilities::isReservedKeyword(splittedOperands[0])){
            this->errorMessage = splittedOperands[0] + " is invalid operand because it's a reserved keyword";
            return false;
        }
        else if(!(ValidatorUtilities::isSymbol(splittedOperands[0],LABEL_MAXLENGTH))
                && !(ValidatorUtilities::isHexAddress(splittedOperands[0],6))){
            this->errorMessage = splittedOperands[0] + " is invalid operand";
            return false;
        }
        else{
            return true;
        }
    }else if(splittedOperands.size()==1){
        string nonPrefixedOperand = splittedOperands[0];
        if(splittedOperands[0][0] == '#' || splittedOperands[0][0] == '@'){
                nonPrefixedOperand = splittedOperands[0].substr(1);
        }
        if(ValidatorUtilities::isRegister(nonPrefixedOperand)){
            this->errorMessage = nonPrefixedOperand + " is invalid operand because it's a register";
            return false;
        }
        else if(ValidatorUtilities::isReservedKeyword(nonPrefixedOperand)){
            this->errorMessage = nonPrefixedOperand + " is invalid operand";
            return false;
        }
        if(!ValidatorUtilities::isSymbol(nonPrefixedOperand,LABEL_MAXLENGTH)
           && !ValidatorUtilities::isHexAddress(nonPrefixedOperand,6) && !(nonPrefixedOperand=="*")){
            this->errorMessage = splittedOperands[0] + " is invalid operand";
            return false;
        }
        else{
            return true;
        }

    }else{
        this->errorMessage = "number of operands must be 1";
        return false;
    }
}

bool SyntaxValidator::checkDirectiveOperand(){
    string directive = sourceLine.getOperation();
    string operand = sourceLine.getOperand();
    if(operand == "" && directive!="END"){
        this->errorMessage="you must specify the operand";
        return false;
    }
    if(directive == "BYTE"){
        if(operand.size()>=3 && operand[1]== '\'' && operand[operand.size()-1]=='\''){
            string actualOperand = operand.substr(2,operand.size()-3);
            if(operand[0]== 'C'){
                if(actualOperand.size()>15){
                    this->errorMessage= actualOperand + " can't exceed 15 characters";
                    return false;
                }
            }
            else if (operand[0]== 'X'){
                if(!ValidatorUtilities::isHexAddress(actualOperand,14)){
                   this->errorMessage= actualOperand + " must be a hexadecimal constant with length at most 14 digits";
                    return false;
                }

            }else{
                this->errorMessage = "operand must be a character string or hexadecimal constant";
                return false;
            }
        }else{
            this->errorMessage = "operand must be a character string or hexadecimal constant";
            return false;
        }
    }else if(directive == "WORD"){
        if(operand[0]==',' || operand[operand.size()-1]==','){
        errorMessage = operand + " is invalid operand";
            return false;
        }
        vector<std::string> splittedOperands = ValidatorUtilities::split(operand,',');
        for(string str : splittedOperands){
            if(!ValidatorUtilities::isSymbol(str,LABEL_MAXLENGTH)
               && !ValidatorUtilities::isDecimalNumber(str,4,true)){
                this->errorMessage = "operand must be a label or a decimal value (up to 4 decimal digits)";
                return false;
            }
        }
    }else if(directive == "RESB" || directive == "RESW"){
        if(!ValidatorUtilities::isDecimalNumber(operand,4,false)){
            this->errorMessage= "operand must be a positive decimal value (up to 4 decimal digits)";
            return false;
        }
        if(stoi(operand)==0){
            this->errorMessage="operand must be a POSITIVE (NOT ZERO) decimal value (up to 4 decimal digits)";
            return false;
        }
    }else if(directive == "START"){
        if(operand==""){
            this->errorMessage= "must specify the starting address of the program";
            return false;
        }
        if(!ValidatorUtilities::isHexAddress(operand,4)){
            this->errorMessage="operand must be a hexadecimal constant (up to 4 digits)";
            return false;
        }
    }else{  // "END"
        if(!ValidatorUtilities::isHexAddress(operand,4) && !(ValidatorUtilities::isSymbol(operand,LABEL_MAXLENGTH)) ){
            this->errorMessage="operand must be a hexadecimal constant (up to 4 digits) or a symbol";
            return false;
        }
    }
    return true;
}

