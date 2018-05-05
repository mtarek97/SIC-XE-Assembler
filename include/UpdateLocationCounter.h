#ifndef UPDATELOCATIONCOUNTER_H
#define UPDATELOCATIONCOUNTER_H
#include<SourceLine.h>

class UpdateLocationCounter
{
    public:
        UpdateLocationCounter();
        static int setLocationCounter(int locationCounter, SourceLine sourceLine);
        static int detectStart(int locationCounter, SourceLine sourceLine);
    protected:

    private:
};

#endif // UPDATELOCATIONCOUNTER_H
