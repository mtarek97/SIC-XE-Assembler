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
   this->lengthOfProg = lengthOfProg;
   this->objCodeGenerator = ObjectCodeGenerator::getObjectCodeGenerator();
   int MAX_TEXT_RECORED_LENGTH = 60;
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
        ///what I should do when there is not start?
    }


    currentLine = sourceLinesArr[linesCounter];
    TextStartAddress = convertToHEX(currentLine.getLocationCounter());
    TextRecord = "";

    vector <string> modificationAddress;
    vector <string> modificationLength;
    ///what should i do if there isn't end?
    while(currentLine.getOperation() != "END" && linesCounter < sourceLinesArr.size()){
        if(currentLine.getIsValid() && currentLine.getHasObjCode() && currentLine.getOperation() != ""){
           string opCode = objCodeGenerator->getObjectCode(currentLine);
           if(opCode != ObjectCodeGenerator::SOME_THING_WRONG){
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
           }else{
               ///boolean to set pass2 error after if conditions.(will be done!).
           }
        }else if(currentLine.getOperation() == "BASE"){
            objCodeGenerator->setBaseStatus(true);
            objCodeGenerator->setCurrentBaseAddress(currentLine.getOperand());
        }else if(currentLine.getOperation() == "NOBASE"){
            objCodeGenerator->setBaseStatus(false);
        }
        ///write listing file(it will be done!).
        if(!currentLine.getIsValid()){
            ///write error in pass1 .how to get error from pass1?(it will be done!).
        }
        /// if there error in pass2 write error in file (will be done!).
        prevLine = currentLine;
        currentLine = sourceLinesArr[++linesCounter];
    }
    /// loop on literal after end.
    SourceLine endSourceLine ;
    if(linesCounter < sourceLinesArr.size()-1){
        endSourceLine  = currentLine;
        prevLine = currentLine;
        prevLine.setNextInstruction(prevLine.getLocationCounter());
//        currentLine[++linesCounter];
    }
    while(linesCounter < sourceLinesArr.size()){
        if(currentLine.getIsValid() && currentLine.getHasObjCode() && currentLine.getOperation() != ""){
           string opCode = objCodeGenerator->getObjectCode(currentLine);
           if(opCode != ObjectCodeGenerator::SOME_THING_WRONG){
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
           }else{
               ///boolean to set pass2 error after if conditions.(will be done!).
           }
        }else if(currentLine.getOperation() == "BASE"){
            objCodeGenerator->setBaseStatus(true);
            objCodeGenerator->setCurrentBaseAddress(currentLine.getOperand());
        }else if(currentLine.getOperation() == "NOBASE"){
            objCodeGenerator->setBaseStatus(false);
        }
        ///write listing file(it will be done!).
        if(!currentLine.getIsValid()){
            ///write error in pass1 .how to get error from pass1?(it will be done!).
        }
        /// if there error in pass2 write error in file (will be done!).
        prevLine = currentLine;
        currentLine = sourceLinesArr[++linesCounter];
    }

    /// write last record
    objectProgram.writeText(TextStartAddress,convertToHEX(TextRecord.length()),TextRecord);
    for(int i = 0; i < modificationAddress.size(); i++){
        objectProgram.writeModification(modificationAddress[i],modificationLength[i]);
    }



    /// end record who calculate the address of beginning.

    ///write listing file(it will be done!).the end directive
}
string Pass2::convertToHEX(int num){
    std::stringstream stream;
    stream << std::hex << num;
    return stream.str();
}

void Pass2::writeInFile(SourceLine sourceLine, int error){

}
void Pass2::writeInFileHeaders(){

}
