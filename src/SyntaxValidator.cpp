#include "SyntaxValidator.h"

const int SyntaxValidator::INVALID = 0;
const int SyntaxValidator::INSTRUCTION = 1;
const int SyntaxValidator::DIRECTIVE = 2;
const int SyntaxValidator::LABEL_MAXLENGTH = 8;

// constructor
SyntaxValidator::SyntaxValidator(){
    this->opCodeTable = OpCodeTable::getOpTable();
    this->registers = RegistersTable::getARegistersTable();
}

bool SyntaxValidator::isValid(SourceLine* srcLine){
    this->sourceLine = ValidatorUtilities::toUpperCase(srcLine);
    if(sourceLine->getOperation()=="END" && sourceLine->getLable()!=""){
        this->sourceLine->setErrorMessage("label must be blank at END statement");
        return false;
    }else if(sourceLine->getOperation()=="ORG" && sourceLine->getLable()!=""){
        this->sourceLine->setErrorMessage("label must be blank at ORG statement");
        return false;
    }else{
        if(ValidatorUtilities::isReservedKeyword(sourceLine->getLable())
            || registers->isARegister(sourceLine->getLable())){
            this->sourceLine->setErrorMessage("label can't be a reserved keyword");
            return false;
        }else if(!ValidatorUtilities::isSymbol(sourceLine->getLable(),LABEL_MAXLENGTH)){
            this->sourceLine->setErrorMessage(sourceLine->getLable() + " is invalid. label must start with a letter and consist of letters, numbers, or '_'");
            return false;
        }else{
            int operationType = checkOperation(this->sourceLine->getOperation());
            if(operationType == this->INSTRUCTION){
                OpInfo info = opCodeTable->getInfo(this->sourceLine->getOperation());
                if(info.getFormatBytes() == 2){
                    return this->checkFormat2Operand(info);
                }else{  // format 3 or 4
                    return this->checkFormat3or4Operand(info);
                }
            }else if(operationType == this->DIRECTIVE){
                    sourceLine->setHasObjCode(false);
                    return this->checkDirectiveOperand();
            }else{  // not valid operation
                return false;
            }
        }
    }
    return true;
}

int SyntaxValidator::checkOperation(std::string operation){
    // check for format 4 (+op)
    bool prefixedByPlus = false;
    std::string nonPrefixedOperation;
    if(operation[0]=='+'){
        prefixedByPlus = true;
        nonPrefixedOperation = operation.substr(1);
        this->sourceLine->setOperation(nonPrefixedOperation);
    }else{
        nonPrefixedOperation = operation;
    }
    OpInfo info = opCodeTable->getInfo(nonPrefixedOperation);
    if(info.getOpCode() != OpInfo::NOT_FOUND){
        if(info.getFormatBytes() == 2 && prefixedByPlus){
            this->sourceLine->setErrorMessage("can't use format 4 with " + nonPrefixedOperation);
            return INVALID;
        }
        return INSTRUCTION;
    }
    else if(ValidatorUtilities::isDirective(operation)){
        return DIRECTIVE;
    }else{
        this->sourceLine->setErrorMessage(operation + " is invalid operation");
        return INVALID;
    }
}

bool SyntaxValidator::checkFormat2Operand(OpInfo info){
    string operand =  sourceLine->getOperand();
    if(operand[0]==',' || operand[operand.size()-1]==','){
        sourceLine->setErrorMessage(operand + " is invalid operand");
            return false;
    }
    vector<std::string> splittedOperands = ValidatorUtilities::split(operand,',');
    int noOfOperands = info.getNumberOfOperands();
    if(splittedOperands.size() == noOfOperands){
        for(int i=0;i<noOfOperands;i++){
            if(!registers->isARegister(splittedOperands[i])){
                sourceLine->setErrorMessage(splittedOperands[i] + " is invalid register");
                return false;
            }
        }
    }else{
        this->sourceLine->setErrorMessage("number of operands must be " + to_string(noOfOperands));
        return false;
    }
    return true;
}

bool SyntaxValidator::checkFormat3or4Operand(OpInfo info){
    if(sourceLine->getOperation() == "RSUB"){
        if(sourceLine->getOperand() != ""){
            this->sourceLine->setErrorMessage("RSUB can't take any operand");
            return false;
        }else{
            return true;
        }
    }
    string operand = sourceLine->getOperand();
    if(operand[0]==',' || operand[operand.size()-1]==','){
        sourceLine->setErrorMessage(operand + " is invalid operand");
            return false;
    }
    vector<std::string> splittedOperands = ValidatorUtilities::split(operand,',');
    if(splittedOperands.size()==2){
        if(!(splittedOperands[1]=="X")){
            this->sourceLine->setErrorMessage("can't use register rather than X to indexed addressing");
            return false;
        }
        else if(registers->isARegister(splittedOperands[0])){
            this->sourceLine->setErrorMessage("can't use registers with indexed addressing") ;
            return false;
        }
        else if(ValidatorUtilities::isReservedKeyword(splittedOperands[0])){
            this->sourceLine->setErrorMessage(splittedOperands[0] + " is invalid operand because it's a reserved keyword");
            return false;
        }
        else if(!(ValidatorUtilities::isSymbol(splittedOperands[0],LABEL_MAXLENGTH))
                && !(ValidatorUtilities::isHexAddress(splittedOperands[0],6))){
            this->sourceLine->setErrorMessage(splittedOperands[0] + " is invalid operand");
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
        if(registers->isARegister(nonPrefixedOperand)){
            this->sourceLine->setErrorMessage(nonPrefixedOperand + " is invalid operand because it's a register");
            return false;
        }
        else if(ValidatorUtilities::isReservedKeyword(nonPrefixedOperand)){
            this->sourceLine->setErrorMessage(nonPrefixedOperand + " is invalid operand");
            return false;
        }
        if(!ValidatorUtilities::isSymbol(nonPrefixedOperand,LABEL_MAXLENGTH)
           && !ValidatorUtilities::isHexAddress(nonPrefixedOperand,6) && !(nonPrefixedOperand=="*")){
            this->sourceLine->setErrorMessage(splittedOperands[0] + " is invalid operand");
            return false;
        }
        else{
            return true;
        }

    }else{
        this->sourceLine->setErrorMessage("number of operands must be 1");
        return false;
    }
}

bool SyntaxValidator::checkDirectiveOperand(){
    string directive = sourceLine->getOperation();
    string operand = sourceLine->getOperand();
    if(operand == "" && directive!="END"){
        this->sourceLine->setErrorMessage("you must specify the operand");
        return false;
    }
    if(directive == "BYTE"){
        sourceLine->setHasObjCode(true);
        if(operand.size()>=3 && operand[1]== '\'' && operand[operand.size()-1]=='\''){
            string actualOperand = operand.substr(2,operand.size()-3);
            if(operand[0]== 'C'){
                if(actualOperand.size()>15){
                    this->sourceLine->setErrorMessage(actualOperand + " can't exceed 15 characters");
                    return false;
                }
            }
            else if (operand[0]== 'X'){
                if(!ValidatorUtilities::isHexAddress(actualOperand,14)){
                   this->sourceLine->setErrorMessage(actualOperand + " must be a hexadecimal constant with length at most 14 digits");
                    return false;
                }

            }else{
                this->sourceLine->setErrorMessage("operand must be a character string or hexadecimal constant");
                return false;
            }
        }else{
            this->sourceLine->setErrorMessage("operand must be a character string or hexadecimal constant");
            return false;
        }
    }else if(directive == "WORD"){
        sourceLine->setHasObjCode(true);
        if(operand[0]==',' || operand[operand.size()-1]==','){
        sourceLine->setErrorMessage(operand + " is invalid operand");
            return false;
        }
        vector<std::string> splittedOperands = ValidatorUtilities::split(operand,',');
        for(string str : splittedOperands){
            if(!ValidatorUtilities::isSymbol(str,LABEL_MAXLENGTH)
               && !ValidatorUtilities::isDecimalNumber(str,4,true)){
                this->sourceLine->setErrorMessage("operand must be a label or a decimal value (up to 4 decimal digits)");
                return false;
            }
        }
    }else if(directive == "RESB" || directive == "RESW"){
        if(!ValidatorUtilities::isDecimalNumber(operand,4,false)){
            this->sourceLine->setErrorMessage("operand must be a positive decimal value (up to 4 decimal digits)");
            return false;
        }
        if(stoi(operand)==0){
            this->sourceLine->setErrorMessage("operand must be a POSITIVE (NOT ZERO) decimal value (up to 4 decimal digits)");
            return false;
        }
    }else if(directive == "START"){
        if(operand==""){
            this->sourceLine->setErrorMessage("must specify the starting address of the program");
            return false;
        }
        if(!ValidatorUtilities::isHexAddress(operand,4)){
            this->sourceLine->setErrorMessage("operand must be a hexadecimal constant (up to 4 digits)");
            return false;
        }
    }else if (directive == "END"){  // "END"
        if(!ValidatorUtilities::isHexAddress(operand,4) && !(ValidatorUtilities::isSymbol(operand,LABEL_MAXLENGTH)) ){
            this->sourceLine->setErrorMessage("operand must be a hexadecimal constant (up to 4 digits) or a symbol");
            return false;
        }
    } else if(directive == "EQU" || directive == "ORG"){
        if(!ValidatorUtilities::isSymbol(operand,LABEL_MAXLENGTH) &&
           !(ValidatorUtilities::isDecimalNumber(operand,7,false) && stoi(operand)<=2e20)){
            this->sourceLine->setErrorMessage("operand must be a symbol or a constant number");
            return false;
        }
    }
    return true;
}

