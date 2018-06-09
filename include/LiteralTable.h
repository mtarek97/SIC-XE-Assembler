#ifndef LITERALTABLE_H
#define LITERALTABLE_H
#include<bits/stdc++.h>
#include <map>
class LiteralTable
{
    public:
        void SetLiteralsTable(std::map<std::string,std::pair< bool, int> > newTable);
        static LiteralTable* getLiteralsTable();
         std::map<std::string,std::pair< bool, int> > getTable();


    private:
        std::map<std::string,std::pair< bool, int> > table;
        static LiteralTable* uniqueInstance;
        LiteralTable();

};

#endif // LITERALTABLE_H
