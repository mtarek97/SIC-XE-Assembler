#include "DetectLiterals.h"
#include "UpdateLocationCounter.h"
#include <bits/stdc++.h>
#include "AssemblyListing.h"
#include "SourceLine.h"
#include "LiteralTable.h"
DetectLiterals::DetectLiterals()
{

}
vector<SourceLine> DetectLiterals::detect(int locationCounter, map<string, bool> lieralTable)
{

    vector<SourceLine> newLines;

    map<string, bool>::iterator it = lieralTable.begin();
    while (it != lieralTable.end())
    {
        if(it->second == true){
            it++;
            continue;
        }
        SourceLine newline;
        newline.setLocationCounter(locationCounter);
        newline.setOperand(it->first);
        newline.setOperation("*");
        newline.setHasObjCode(true);
        newLines.push_back(newline);
        locationCounter = UpdateLocationCounter::setLocationCounter(locationCounter, newline, nullptr).first;
        it++;
    }
    return newLines;
}

