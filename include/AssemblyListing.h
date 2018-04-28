#ifndef ASSEMBLYLISTING_H
#define ASSEMBLYLISTING_H
#include<SourceLine.h>
#include<bits/stdc++.h>

class AssemblyListing
{
    public:
        AssemblyListing();
        void write(SourceLine sourceLine, string locationCounter, string error);

    protected:

    private:
        int counter = 0;
};

#endif // ASSEMBLYLISTING_H
