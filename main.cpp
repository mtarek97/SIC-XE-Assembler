#include <iostream>

#include "SymbolTable.h"

using namespace std;

int main()
{
    // testing symbol table
    SymbolTable symtab;
    symtab.insert("var1", 1000);
    symtab.insert("var2", 1003);
    cout << "var1 = " << symtab.search("var1") << endl;
    cout << "var2 = " << symtab.search("var2") << endl;
    cout << "var3 = " << symtab.search("var3") << endl;
    return 0;
}
