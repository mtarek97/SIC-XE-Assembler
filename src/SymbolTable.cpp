#include "SymbolTable.h"
#include<bits/stdc++.h>
#include <string>
#include<fstream>
#include<iostream>

using namespace std;

SymbolTable* SymbolTable::uniqueInstance = 0;

SymbolTable::SymbolTable(){
    //...
}

SymbolTable* SymbolTable::getSymbolTable() {
    if(!uniqueInstance){
        uniqueInstance = new SymbolTable();
    }
    return uniqueInstance;
}

void SymbolTable::insert(string symbol, int locInMemory){
    this->hashtable.insert(pair<string, SymbolInfo> (symbol, SymbolInfo(locInMemory, 'r')));
    this->inserted.insert(pair<string, bool> (symbol, true));
    SymbolTable::writeInFile(symbol,locInMemory);
}

SymbolInfo SymbolTable::search(string symbol){
  int jj =     (hashtable.count(symbol)  );

    if(hashtable.count(symbol) != 0 ){
        return this->hashtable[symbol];
    }
    else{
        return SymbolInfo(-1, 'e'); // symbol not found
    }
}

void SymbolTable::writeInFile(string symbol, int locInMemory)
{
    if(isFirst){
        ofstream out("SYMTAB.txt",ios::out);
        std:: string line(" Loc   SYMBOL\n");
        out << line;
        out.close();
        isFirst = false;
    }
        std:: string line("");
        std::string loc = to_string(locInMemory);
        std::string zeros = "";
        int i = 0;
        for(i = 0; i < 5 - loc.length(); i++){
            zeros = zeros + "0";
        }
        loc = zeros + loc;
        line = loc + "  " + symbol + "\n";
        ofstream out("SYMTAB.txt",ios::out | ios::app);
        out << line;
        out.close();


}
