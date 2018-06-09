#ifndef DETECTLITERALS_H
#define DETECTLITERALS_H
#include "SourceLine.h"
#include <bits/stdc++.h>
class DetectLiterals
{
    public:
        DetectLiterals();
        static vector<SourceLine> detect(int locationCounter, std::map<std::string,std::pair< bool, int> >lieralTable);
    private:
};

#endif // DETECTLITERALS_H
