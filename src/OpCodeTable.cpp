#include "OpCodeTable.h"
#include <iostream>
#include <fstream>

using namespace std;

const string OpCodeTable::filePath = "opTable.txt";

OpCodeTable* OpCodeTable::uniqueInstance = 0;

OpCodeTable::OpCodeTable()
{
    ifstream infile;
    infile.open (this->filePath);
    string operationKey, opCode;
    int formatBytes;
    while(infile >> operationKey >> opCode >> formatBytes){
        this->opTable.insert(pair<string, OpInfo> (operationKey, OpInfo(opCode, formatBytes)));
    }
    infile.close();
}

OpCodeTable* OpCodeTable::getOpTable() {
    if(!uniqueInstance){
        uniqueInstance = new OpCodeTable();
    }
    return uniqueInstance;
}

OpInfo OpCodeTable::getInfo(std::string key) {
    std::unordered_map<std::string,OpInfo>::const_iterator got = this->opTable.find(key);
    if ( got == this->opTable.end())
        return OpInfo("11" , 0);
    else
        return got->second;
}