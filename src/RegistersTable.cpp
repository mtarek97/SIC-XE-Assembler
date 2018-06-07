//
// Created by Mohamed Mashaal on 6/6/2018.
//
#include "RegistersTable.h"

RegistersTable* RegistersTable::uniqueInstance = 0;

RegistersTable::RegistersTable() {
    this->registersMap.insert(pair<string,int>("A",0));
    this->registersMap.insert(pair<string,int>("X",1));
    this->registersMap.insert(pair<string,int>("L",2));
    this->registersMap.insert(pair<string,int>("PC",8));
    this->registersMap.insert(pair<string,int>("SW",9));
    this->registersMap.insert(pair<string,int>("B",3));
    this->registersMap.insert(pair<string,int>("S",4));
    this->registersMap.insert(pair<string,int>("T",5));
    this->registersMap.insert(pair<string,int>("F",6));
}

RegistersTable* RegistersTable::getARegistersTable() {
    if(!uniqueInstance){
        uniqueInstance = new RegistersTable();
    }
    return uniqueInstance;
}

int RegistersTable::getRegisterNumber(string r) {
    if(isARegister(r))
        return this->registersMap[r];
    return -1;
}

bool RegistersTable::isARegister(string r) {
    if(this->registersMap.find(r) == this->registersMap.end())
        return false;
    return true;
}



