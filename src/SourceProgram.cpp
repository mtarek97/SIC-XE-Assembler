#include "SourceProgram.h"
#include<SourceLine.h>
#include<string.h>
#include<bits/stdc++.h>
#include"OpCodeTable.h"
SourceProgram::SourceProgram()
{


}
void SourceProgram::parse(string fileName)
{
    freopen("aaa.txt", "r", stdin);
    string parser, word;
    bool stop = false;
    this->locationCounter=0;
    vector<string> line;
    while(!stop)
    {
        getline(cin,parser);
        stringstream strem(parser);
        if(!(strem>>word))
        {
            stop = true;  // END OF FILE
            continue;
        }
        line.push_back(word);
        while(strem>>word)
        {
            line.push_back(word);
        }
        SourceLine sourceLine;
        // if(validYaTarek(sourceLine)) {
        if(!isComment(line))
        {
            sourceLine = identifier(line);
            std::transform(sourceLine.getOperation().begin(), sourceLine.getOperation().end(), sourceLine.getOperation().begin(), ::tolower);

            if(sourceLine.getOperation() == "start")
            {
                this->locationCounter = std::stoi(sourceLine.getOperand());
            }

        }
   // }
    else  // Comment Line
    {
       sourceLine.setComment(line[0]);
       sourceLine.setLable("");
       sourceLine.setOperand("");
       sourceLine.setOperation("");

    }






}
}

SourceLine SourceProgram::identifier(vector<string> line)
{

    int index = 0;
    SourceLine sourceLine;
    OpCodeTable* opCodeTable = OpCodeTable::getOpTable();
    OpInfo opinfo = opCodeTable->getInfo(line[index]);
    if(opinfo.getOpCode() == "11")
    {
        sourceLine.setComment(line[index]);
        index++;
    }
    sourceLine.setOperation(line[index++]);
    if(index != line.size())
        sourceLine.setOperand(line[index++]);
    if(index != line.size())
        sourceLine.setComment(line[index++]);
    return sourceLine;
}

bool SourceProgram::isComment(vector<string>line)
{
    if(line[0][0] == '.')
        return true;
    return false;

}




