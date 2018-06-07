//
// Created by Mohamed Mashaal on 4/25/2018.
//
#include "OpInfo.h"

const std::string OpInfo::NOT_FOUND = "11";

std::string OpInfo::getOpCode(){
    return this->opCode;
}

int OpInfo::getFormatBytes() {
    return this->formatBytes;
}

OpInfo::OpInfo(std::string opCode, int formatBytes , int operands) {
    this->opCode = opCode;
    this->formatBytes = formatBytes;
    this-> operands = operands;
}

int OpInfo::getNumberOfOperands() {
    return this->operands;
}