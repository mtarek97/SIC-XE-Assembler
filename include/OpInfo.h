//
// Created by Mohamed Mashaal on 4/25/2018.
//

#ifndef SIC_XE_ASSEMBLER_OPINFO_H
#define SIC_XE_ASSEMBLER_OPINFO_H
#include <string>
class OpInfo{
    public:
        std::string getOpCode();
        int getFormatBytes();
        OpInfo(std::string opCode, int formatBytes, int operands);
         static const std::string NOT_FOUND;
         int getNumberOfOperands();
    private:
        std::string opCode;
        int formatBytes;
        int operands;
};
#endif //SIC_XE_ASSEMBLER_OPINFO_H
