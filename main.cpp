#include <iostream>

#include "SymbolTable.h"
#include "OpCodeTable.h"
#include "SourceLine.h"
#include "SyntaxValidator.h"
#include<SourceProgram.h>
using namespace std;

int main()
{
    // testing symbol table
    /*SymbolTable symtab;
    symtab.insert("var1", 1000);
    symtab.insert("var2", 1003);
    cout << "var1 = " << symtab.search("var1") << endl;
    cout << "var2 = " << symtab.search("var2") << endl;
    cout << "var3 = " << symtab.search("var3") << endl;*/
    /*
    OpCodeTable* opTable = OpCodeTable::getOpTable();
    OpInfo info =  opTable->getInfo("START");
    if(info.getOpCode()== info.NOT_FOUND)
        cout << "Not found" << "\n";
        else
            cout<<"Found"<<endl;

    info = opTable->getInfo("MUL");
    cout << info.getOpCode() << " " << info.getFormateBytes() << " " << info.getNumberOfOperands();*/

    SourceLine srcLine;
    srcLine.setLable("");
    srcLine.setOperation("BYTE");
    srcLine.setOperand("C'shazly   ya rb'");
    /*SourceLine srcLine;
    srcLine.setLable("mah");
    srcLine.setOperation("MULR");
    srcLine.setOperand("R,X");
    SyntaxValidator validator;
    if(validator.isValid(srcLine)){
        cout << "valid source line" << endl;
    }else{
        cout << validator.getErrorMessage() << endl;
    }*/
    SourceProgram sourceProgram;
    string l = "aaa.txt";
    sourceProgram.parse(&l[0]);

    return 0;
}
