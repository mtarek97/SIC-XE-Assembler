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

std::map<std::string,std::pair< bool, int> > LiteralTable::getTable() {
        return table;
}

void LiteralTable::SetLiteralsTable(std::unordered_map<std::string,std::pair< bool, int> > newTable) {
   table.clear();
   std::unordered_map<std::string,std::pair< bool, int> >::iterator it = newTable.begin();
    while (it != newTable.end())
    {
    table.insert(std::pair<std::string,std::pair< bool, int> >(it->first,it->second));
    it++;
    }

}
