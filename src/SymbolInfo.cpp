#include "SymbolInfo.h"

SymbolInfo::SymbolInfo(int locInMemory, char type)
{
    this->location = locInMemory;
    this->symbolType = type;
}

int getLocation(){
    return this->location;
}


char getType(){
    return this->symbolType;
}
