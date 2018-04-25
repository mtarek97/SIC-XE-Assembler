//
// Created by Mohamed Mashaal on 4/25/2018.
//
#include "OpInfo.h"

const std::string NOT_FOUND = "11";

std::string OpInfo::getOpCode(){
    return this->opCode;
}

int OpInfo::getFormateBytes() {
    return this->formatBytes;
}

OpInfo::OpInfo(std::string opCode, int formateBytes) {
    this->opCode = opCode;
    this->formatBytes = formateBytes;
}

int OpInfo::getNumberOfOperands() {
    
}