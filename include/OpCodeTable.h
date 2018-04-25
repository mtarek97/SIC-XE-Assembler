#ifndef OPCODETABLE_H
#define OPCODETABLE_H

#include <string>
#include "OpInfo.h"
#include <unordered_map>

class OpCodeTable
{
    public:
        static OpCodeTable* getOpTable();
        OpInfo getInfo(std::string key);
    protected:

    private:
        static OpCodeTable* uniqueInstance;
        OpCodeTable();
        std::unordered_map<std::string, OpInfo> opTable;
        static const std::string filePath;
};

#endif  //OPCODETABLE_H
