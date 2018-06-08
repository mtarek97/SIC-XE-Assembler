#include <iostream>
#include <ObjectCodeGenerator.h>

#include "SymbolTable.h"
#include "OpCodeTable.h"
#include "SourceLine.h"
#include "SyntaxValidator.h"
#include "SourceProgram.h"
using namespace std;

/*int main( int argc, char *argv[] ){

    SourceProgram sourceProgram;
    string l = argv[2];
    sourceProgram.parse(&l[0]);
    return 0;
}*/

int main( )
{

    // testing symbol table
    /*SymbolTable symtab;
    symtab.insert("var1", 1000);
    symtab.insert("var2", 1003);
    cout << "var1 = " << symtab.search("var1") << endl;
    cout << "var2 = " << symtab.search("var2") << endl;
    cout << "var3 = " << symtab.search("var3") << endl;*/

    /*OpCodeTable* opTable = OpCodeTable::getOpTable();
    OpInfo info =  opTable->getInfo("START");
    if(info.getOpCode()== info.NOT_FOUND)
        cout << "Not found" << "\n";
        else
            cout<<"Found"<<endl;

    info = opTable->getInfo("MUL");
    cout << info.getOpCode() << " " << info.getFormateBytes() << " " << info.getNumberOfOperands();*/

    /*SourceLine srcLine;
    srcLine.setLable("");
    srcLine.setOperation("BYTE");
    srcLine.setOperand("C'shazly   ya rb'");*/
    /*SourceLine srcLine;
    srcLine.setLable("");
    srcLine.setOperation("END");
    srcLine.setOperand("bgn");
    srcLine.setOperand("");
    SyntaxValidator validator;
    if(validator.isValid(srcLine)){
        cout << "valid source line" << endl;
    }else{
        cout << validator.getErrorMessage() << endl;
    }*/
    //SourceProgram sourceProgram;
    //}
    /*SourceProgram sourceProgram;
    string l = "aaa.txt";
    sourceProgram.parse(&l[0]);*/
    string fileName;
    cout<<"Enter File Name\n";
    cin>>fileName;
    SourceProgram sourceProgram;
    string l = "aaaa.txt";
    sourceProgram.parse(&fileName[0]);

    /*
        ObjectCodeGenerator Guide :
        ObjectCodeGenerator* generator = ObjectCodeGenerator::getObjectCodeGenerator(); getting instance
        generator->setSymbolTable(symbolTable); you must set the symbol table after finishing pass one so that the generator can calculate displacement
        string result = generator->getObjectCode(sourceLine); returns the objectCode representing this Instruction. SourceLine Object (NextInstruction member variable)
        should be filled as it is needed in the calculation of the displacement.
        Next Instruction is set in int not HEX (same as location counter being kept in code currently).
        Almost no validation is being done, so don't pass for example RESW, REWSB or EQU those Directives have no objectcode !
        Literals is missing, since no literal locations is found since there is no literal handling yet!
        Also EQU should be added may be to symbol table because of the displacement calculations.
    */
    ObjectCodeGenerator* generator = ObjectCodeGenerator::getObjectCodeGenerator();

    SourceLine sourceLine ; // symbol Table has been set inside source Program for testing purposes only.
    sourceLine.setOperation("EQU");
    sourceLine.setOperand("BUFFER+10");
    sourceLine.setNextInstruction(4177);

    SyntaxValidator validator;
    if(validator.isValid(&sourceLine)){
        cout << "valid source line" << endl;
    }else{
        sourceLine.setIsValid(false);
        cout << sourceLine.getErrorMessage() << endl;
    }
    cout << (sourceLine.getIsValid() ? "valid syntax" : "invalid syntax") << endl;
    cout << (sourceLine.getHasObjCode() ? "has obj code" : "has no obj code") << endl;

    string result = generator->getObjectCode(sourceLine);
    cout << result << endl;

    return 0;
}
