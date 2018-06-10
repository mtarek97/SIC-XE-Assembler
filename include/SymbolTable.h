#include <string>
#include <map>
#include "SymbolInfo.h"

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

using namespace std;

class SymbolTable{

    public:
        static SymbolTable* getSymbolTable();
        void insert(string symbol, int locInMemory);
        SymbolInfo search(string symbol);
        map<string, SymbolInfo> hashtable;


    private:
        SymbolTable();
        static SymbolTable* uniqueInstance;
        map<string, bool> inserted;
        bool isFirst;
        void writeInFile(string symbol, string locInMemory);
};

#endif // SYMBOLTABLE_H
