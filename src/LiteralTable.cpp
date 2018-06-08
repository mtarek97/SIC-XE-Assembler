#include "LiteralTable.h"
LiteralTable* LiteralTable::uniqueInstance = 0;

LiteralTable::LiteralTable()
{
    //ctor
}
LiteralTable* LiteralTable::getLiteralTable() {
    if(!uniqueInstance){
        uniqueInstance = new LiteralTable();
    }
    return uniqueInstance;
}

