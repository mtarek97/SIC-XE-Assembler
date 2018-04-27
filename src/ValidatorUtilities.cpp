#include "ValidatorUtilities.h"

using namespace std;

const std::string ValidatorUtilities::registers[] = {"A","X","L","B","S","T","F","PC","SW"};
const std::string ValidatorUtilities::directives[] = {"BYTE","WORD","RESB","RESW","START","END"};
OpCodeTable* ValidatorUtilities::opCodeTable = OpCodeTable::getOpTable();

// private constructor --static class
ValidatorUtilities::ValidatorUtilities(){}


vector<string> ValidatorUtilities::split(string str, char delimiter) {
    stringstream strStream(str);
    string segment;
    vector<string> tokens;
    while (getline(strStream, segment, delimiter)) {
        tokens.push_back(segment);
    }
    return tokens;
}

bool ValidatorUtilities::isSymbol(string str,int maxLength){
    if(str.size() > maxLength){
        return false;
    }
    if(isdigit(str[0])){
        return false;
    }
    for(int i=0;i<str.size();i++){
        if(!isalpha(str[i]) && !isdigit(str[i]) && !(str[i]=='_')){
            return false;
        }
    }
    return true;
}

bool ValidatorUtilities::isReservedKeyword(string str){
    if(isDirective(str)){
        return true;
    }
    OpInfo info = opCodeTable->getInfo(str);
    if(info.getOpCode() != OpInfo::NOT_FOUND){
        return true;
    }
    return false;
}

bool ValidatorUtilities::isHexAddress(string str, int maxLength){
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

bool ValidatorUtilities::isRegister(string str){
    if(find(registers,registers+9,str) == registers+9){
        return false;
    }
    return true;
}

bool ValidatorUtilities::isDirective(string str){
    if(find(directives,directives+6,str) == directives+6){
        return false;
    }
    return true;
}

bool ValidatorUtilities::isDecimalNumber(string str, int maxDigitsCount, bool canBeNegative){
    string absolute = str;
    bool isNegative = false;
    if(str[0]=='-'){
        absolute = str.substr(1);
        isNegative = true;
    }
    if(isNegative && !canBeNegative){
        return false;
    }
    if(absolute.size() > maxDigitsCount){
        return false;
    }
    for(int i=0;i<absolute.size();i++){
        if(!isdigit(str[i])){
            return false;
        }
    }
    return true;
}


