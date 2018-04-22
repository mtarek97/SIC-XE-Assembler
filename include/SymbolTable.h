#include <string>
#include <map>

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

using namespace std;

class SymbolTable{

    public:
        SymbolTable();
        void insert(string symbol, int locInMemory);
        int search(string symbol);

    private:
        map<string, int> hashtable;
        map<string, bool> inserted;
};

#endif // SYMBOLTABLE_H
