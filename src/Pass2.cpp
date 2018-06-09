#include "Pass2.h"
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
#include "ObjectProgram.h"
/**
by Mohamed Esmail
*/
Pass2::Pass2(vector<SourceLine> sourceLinesArr,int lengthOfProg)
{
   this->sourceLinesArr = sourceLinesArr;
   this->lengthOfProg =lengthOfProg;
   this->objCodeGenerator = ObjectCodeGenerator::getObjectCodeGenerator();
   this->objCodeGenerator ->setSymbolTable(symbolTable);
}

void Pass2::generateObjProg(){
    linesCounter = 0;
    SourceLine currentLine = sourceLinesArr[linesCounter];
    SourceLine prevLine = sourceLinesArr[linesCounter];
    if(currentLine.getOperation() == "START"){
        ///write listing file(it will be done!).

        string lengthOfProgHex = convertToHEX(lengthOfProg);
        string startAdd = std::to_string(currentLine.getLocationCounter());
        objectProgram.writeHeader(currentLine.getLable(),startAdd,lengthOfProgHex);
        linesCounter++;
    }else{
        ///what I should do when there is not start
    }


    currentLine = sourceLinesArr[linesCounter];
    TextStartAddress = std::to_string(currentLine.getLocationCounter())
    TextRecord = "";

    vector <string> modificationAddress;
    vector <string> modificationLength;
    while(currentLine.getOperation() != "END"){
        if(currentLine.getIsValid() && currentLine.getHasObjCode() && currentLine.getOperation() != ""){
           string opCode = generator->getObjectCode(sourceLine);
           if(prevLine.getNextInstruction() != currentLine.getLOC()){
                objectProgram.writeText(TextStartAddress,convertToHEX(TextRecord.length()),TextRecord);
                TextRecord = opCode;
                TextStartAddress = std::to_string(currentLine.getLOC());
           }else if(TextRecord.length() + opCode.length() <= MAX_TEXT_RECORED_LENGTH){
                TextRecord = TextRecord + opCode;
           }else{
              objectProgram.writeText(TextStartAddress,convertToHEX(TextRecord.length()),TextRecord);
              TextRecord = opCode;
              TextStartAddress = std::to_string(currentLine.getLOC());
           }
           if(currentLine.getOperation()[0] =='+'){
                modificationAddress.push_back(currentLine.getLOC());
                modificationLength.push_back(std::to_string(5));
           }
        }else if(!currentLine.getIsValid()){

        }
        ///write listing file(it will be done!).
        prevLine = currentLine;
        currentLine = sourceLinesArr[++linesCounter];
    }
    objectProgram.writeText(TextStartAddress,convertToHEX(TextRecord.length()),TextRecord);
        ///modification records and end record
        ///write listing file(it will be done!).
}
string Pass2::convertToHEX(int num){
    std::stringstream stream;
    stream << std::hex << num;
    return stream.str();
}
