#ifndef LITERALTABLE_H
#define LITERALTABLE_H
#include<bits/stdc++.h>

class LiteralTable
{
    public:
        static LiteralTable* getLiteralTable();
        std::map<std::string, bool> table;

    private:
        static LiteralTable* uniqueInstance;
        LiteralTable();
};

#endif // LITERALTABLE_H
