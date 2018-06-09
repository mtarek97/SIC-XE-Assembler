#ifndef UPDATELOCATIONCOUNTER_H
#define UPDATELOCATIONCOUNTER_H
#include<SourceLine.h>
#include <SymbolTable.h>
class UpdateLocationCounter
{
    public:
        UpdateLocationCounter();
        static pair<int,string> setLocationCounter(int locationCounter, SourceLine sourceLine);
        static int detectStart(int locationCounter, SourceLine sourceLine);
    protected:

    private:
};

#endif // UPDATELOCATIONCOUNTER_H
