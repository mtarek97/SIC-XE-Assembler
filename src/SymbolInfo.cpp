#include "SymbolInfo.h"

SymbolInfo::SymbolInfo(int locInMemory, char type)
{
    this->location = locInMemory;
    this->symbolType = type;
}

SymbolInfo::SymbolInfo() {
    
}

int SymbolInfo::getLocation(){
    return this->location;
}


char SymbolInfo::getType(){
    return this->symbolType;
}
