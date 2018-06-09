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
    TextStartAddress = convertToHEX(currentLine.getLocationCounter())
    TextRecord = "";

    vector <string> modificationAddress;
    vector <string> modificationLength;
    while(currentLine.getOperation() != "END"){
        if(currentLine.getIsValid() && currentLine.getHasObjCode() && currentLine.getOperation() != ""){
           string opCode = generator->getObjectCode(sourceLine);
           /// if there isn't error in object code.(it will be done!).
           if(prevLine.getNextInstruction() != currentLine.getLocationCounter()){
                objectProgram.writeText(TextStartAddress,convertToHEX(TextRecord.length()),TextRecord);
                TextRecord = opCode;
                TextStartAddress = convertToHEX(currentLine.getLocationCounter());
           }else if(TextRecord.length() + opCode.length() <= MAX_TEXT_RECORED_LENGTH){
                TextRecord = TextRecord + opCode;
           }else{
              objectProgram.writeText(TextStartAddress,convertToHEX(TextRecord.length()),TextRecord);
              TextRecord = opCode;
              TextStartAddress = convertToHEX(currentLine.getLocationCounter());
           }
           if(currentLine.getOperation()[0] == '+'){
                modificationAddress.push_back(convertToHEX(currentLine.getLocationCounter()));
                modificationLength.push_back(std::to_string(5));
           }
           ///write listing file(it will be done!).
        }else if(!currentLine.getIsValid()){
            ///write listing file(it will be done!).
            ///write error(it will be done!).
        }
        prevLine = currentLine;
        currentLine = sourceLinesArr[++linesCounter];
    }
    objectProgram.writeText(TextStartAddress,convertToHEX(TextRecord.length()),TextRecord);
        for(int i = 0; i < modificationAddress.size(); i++){
            objectProgram.writeModification(modificationAddress[i],modificationLength[i]);
        }
        /// end record who calculate
        ///write listing file(it will be done!).the end directive
}
string Pass2::convertToHEX(int num){
    std::stringstream stream;
    stream << std::hex << num;
    return stream.str();
}
