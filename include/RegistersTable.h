//
// Created by Mohamed Mashaal on 6/6/2018.
//

#ifndef SIC_XE_ASSEMBLER_REGISTERSTABLE_H
#define SIC_XE_ASSEMBLER_REGISTERSTABLE_H
using namespace std;
#include <map>

class RegistersTable{
    public:
        static RegistersTable* getARegistersTable();
        bool isARegister(string r);
        int getRegisterNumber(string r);

    private:
        map<string, int> registersMap;
        static RegistersTable* uniqueInstance;
        RegistersTable();
};
#endif //SIC_XE_ASSEMBLER_REGISTERSTABLE_H
