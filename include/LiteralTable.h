#ifndef LITERALTABLE_H
#define LITERALTABLE_H
#include<bits/stdc++.h>
#include <map>
class LiteralTable
{
    public:
        void SetLiteralsTable(std::map<std::string, bool> newTable);
        static LiteralTable* getLiteralsTable();
        std::map<std::string, bool> getTable();


    private:
        std::map<std::string, bool> table;
        static LiteralTable* uniqueInstance;
        LiteralTable();

};

#endif // LITERALTABLE_H
