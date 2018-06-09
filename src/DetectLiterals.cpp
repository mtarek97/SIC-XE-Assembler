#include "DetectLiterals.h"
#include "UpdateLocationCounter.h"
#include <bits/stdc++.h>
#include "AssemblyListing.h"
#include "SourceLine.h"
#include "LiteralTable.h"
DetectLiterals::DetectLiterals()
{

}
vector<SourceLine> DetectLiterals::detect(int locationCounter,  std::map<std::string, std::pair<bool,std::string> > lieralTable)
{

    vector<SourceLine> newLines;

     std::map<std::string, pair<bool,string> >::iterator it = lieralTable.begin();
    while (it != lieralTable.end())
    {
        if(it->second.first == true){
            it++;
            continue;
        }
        SourceLine newline;
        newline.setLocationCounter(locationCounter);
        newline.setOperation(it->first);
        newline.setLable("*");
        newline.setHasObjCode(true);
        newLines.push_back(newline);
        locationCounter = UpdateLocationCounter::setLocationCounter(locationCounter, newline).first;
        it++;
    }
    return newLines;
}

