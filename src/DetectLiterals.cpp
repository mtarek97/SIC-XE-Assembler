#include "DetectLiterals.h"
#include "UpdateLocationCounter.h"
#include <bits/stdc++.h>
#include "AssemblyListing.h"
#include "SourceLine.h"
#include "LiteralTable.h"
DetectLiterals::DetectLiterals()
{

}
vector<SourceLine> DetectLiterals::detect(int locationCounter,  std::unordered_map<std::string, std::pair<bool, int> > lieralTable, std::vector<string> literalVector)
{

    vector<SourceLine> newLines;
for(int i = 0; i < literalVector.size(); i++)
    {
        cout<<literalVector[i]<<"\n";
        if(lieralTable[literalVector[i]].first == true){
            continue;
        }
        SourceLine newline;
        newline.setLocationCounter(locationCounter);
        newline.setOperation(literalVector[i]);
        newline.setLable("*");
        newline.setHasObjCode(true);
        newLines.push_back(newline);
        locationCounter = UpdateLocationCounter::setLocationCounter(locationCounter, newline).first;
    }
    return newLines;
}

