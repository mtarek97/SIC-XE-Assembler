#include "SymbolTable.h"

SymbolTable::SymbolTable(){
    //...
}

void SymbolTable::insert(string symbol, int locInMemory){
    this->hashtable.insert(pair<string, int> (symbol, locInMemory));
    this->inserted.insert(pair<string, bool> (symbol, true));
}

int SymbolTable::search(string symbol){
    if(this->inserted[symbol]){
        return this->hashtable[symbol];
    }
    else{
        return -1; // symbol not found
    }
}
