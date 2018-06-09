#include "LiteralTable.h"
#include<bits/stdc++.h>
LiteralTable* LiteralTable::uniqueInstance = 0;

LiteralTable::LiteralTable()
{

}

LiteralTable* LiteralTable::getLiteralsTable() {
    if(!uniqueInstance){
        uniqueInstance = new LiteralTable();
    }
    return uniqueInstance;
}

std::map<std::string, bool> LiteralTable::getTable() {
        return table;
}

void LiteralTable::SetLiteralsTable(std::map<std::string, bool> newTable) {
   table.clear();
   std::map<std::string, bool>::iterator it = newTable.begin();
    while (it != newTable.end())
    {
    table[it->first] = it->second;
    it++;
    }

}
