#include "ObjectProgram.h"
#include<bits/stdc++.h>
#include<SourceProgram.h>
#include<SourceLine.h>
#include<OpCodeTable.h>
#include<SyntaxValidator.h>
#include<AssemblyListing.h>
#include<SymbolTable.h>
#include<ValidatorUtilities.h>
#include<UpdateLocationCounter.h>
#include <ObjectCodeGenerator.h>
/**
by Mohamed Esmail
*/
ObjectProgram::ObjectProgram()
{
    //ctor
}
void ObjectProgram::writeHeader(string progName, string startingAddress, string lengthOfProg)
{
    ofstream out("ObjectProgram.txt",ios::out);
    std:: string line("H"+progName);
    for(int i = 0; i < 6-progName.length();i++){
        line = line +" ";
    }
    string zeros ="";
    for(int i = 0; i < 6 -startingAddress.length(); i++){
        zeros = zeros + "0";
    }
    startingAddress = zeros + startingAddress;
    line = line + startingAddress;

    zeros ="";
    for(int i = 0; i < 6 -lengthOfProg.length(); i++){
        zeros = zeros + "0";
    }
    lengthOfProg = zeros + lengthOfProg;
    line = line + lengthOfProg +"\n";

    out << line;
    out.close();
}

void ObjectProgram::writeText(string startingAddress, string lengthOFObjCodes, string objCodes)
{
    std:: string line("T");
    string zeros ="";
    for(int i = 0; i < 6 -startingAddress.length(); i++){
        zeros = zeros + "0";
    }
    startingAddress = zeros + startingAddress;
    line = line + startingAddress;

    zeros ="";
    for(int i = 0; i < 2 - lengthOFObjCodes.length(); i++){
        zeros = zeros + "0";
    }
    lengthOFObjCodes = zeros + lengthOFObjCodes;
    line = line + lengthOFObjCodes;

    line = line + objCodes + "\n";

    ofstream out("ObjectProgram.txt",ios::out | ios::app);
    out << line;
    out.close();
}

void ObjectProgram::writeEnd(string executionAddress)
{
    std:: string line("E");
    string zeros ="";
    for(int i = 0; i < 6 -executionAddress.length(); i++){
        zeros = zeros + "0";
    }
    executionAddress = zeros + executionAddress;
    line = line + executionAddress + "\n";

    ofstream out("ObjectProgram.txt",ios::out | ios::app);
    out << line;
    out.close();
}

void ObjectProgram::writeModification(string startingAddress, string Length)
{
    std:: string line("M");
    string zeros ="";
    for(int i = 0; i < 6 -startingAddress.length(); i++){
        zeros = zeros + "0";
    }
    startingAddress = zeros + startingAddress;
    line = line + startingAddress;

    zeros ="";
    for(int i = 0; i < 2 - Length.length(); i++){
        zeros = zeros + "0";
    }
    Length = zeros + Length;
    line = line + Length + "\n";

    ofstream out("ObjectProgram.txt",ios::out | ios::app);
    out << line;
    out.close();
}




