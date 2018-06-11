#ifndef UPDATELOCATIONCOUNTER_H
#define UPDATELOCATIONCOUNTER_H
#include<SourceLine.h>
#include <SymbolTable.h>
#include<bits/stdc++.h>
class UpdateLocationCounter
{
    public:
        UpdateLocationCounter();
        static pair<int,string> setLocationCounter(int locationCounter, SourceLine sourceLine);
        static int detectStart(int locationCounter, SourceLine sourceLine);

    private:
       static bool isInt(std::string operand);

};

#endif // UPDATELOCATIONCOUNTER_H
