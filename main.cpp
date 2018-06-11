#include <iostream>
#include <ObjectCodeGenerator.h>

#include "SymbolTable.h"
#include "OpCodeTable.h"
#include "SourceLine.h"
#include "SyntaxValidator.h"
#include "SourceProgram.h"
#include "Pass2.h"
using namespace std;

/*int main( int argc, char *argv[] ){

    SourceProgram sourceProgram;
    string l = argv[2];
    sourceProgram.parse(&l[0]);
    return 0;
}*/

int main( )
{
    string fileName;
    cout<<"Enter File Name\n";
    cin>>fileName;
    SourceProgram sourceProgram;
    string l = "aaaa.txt";

    Pass2 pass2 = Pass2(sourceProgram.parse(&fileName[0]));
    pass2.generateObjProg();
    return 0;
}
