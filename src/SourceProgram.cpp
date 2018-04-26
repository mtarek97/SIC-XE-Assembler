#include "SourceProgram.h"
#include<SourceLine.h>
#include<string.h>
#include<bits/stdc++.h>
#include"OpCodeTable.h"
SourceProgram::SourceProgram()
{
}
void SourceProgram::parse(char* fileName)
{
    freopen(fileName, "r", stdin);
    string parser, word;
    bool stop = false;
    this->locationCounter=0;
    vector<string> line;
    SourceLine sourceLine;

    getline(cin, parser);
    cout<<parser;
    line = getWords(parser);
    cout<<endl<<line.size();
    sourceLine = identifier(line);
    //if(isvalid)
    string start = sourceLine.getOperation();
    std::transform(start.begin(), start.end(), start.begin(), ::tolower);
    if(start == "start")
    {
        this->locationCounter = std::stoi(sourceLine.getOperand(), nullptr, 2);
    }
    //write
//}
    while(!stop)
    {
        line.clear();
        getline(cin,parser);
        stringstream strem(parser);

        if(!(strem>>word))
        {
            stop = true;  // END OF FILE
            continue;
        }
        line = getWords(parser);
//   We Collect all words in vector called line


        SourceLine sourceLine;
        // if(validYaTarek(sourceLine)) {
        if(!isComment(line))
        {
            sourceLine = identifier(line);
            OpCodeTable* opCodeTable = OpCodeTable::getOpTable();
            OpInfo opinfo = opCodeTable->getInfo(sourceLine.getOperation());

            int x= stoi((opinfo.getOpCode()), nullptr, 2);
            locationCounter+=x;
            if(sourceLine.getOperation()[0] == '+'){
                locationCounter++;
            }
            //write

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
    cout<<line[index]<<endl;
    if(opinfo.getOpCode() == "11")
    {
        sourceLine.setLable(line[index]);
        cout<<"Lable";
        index++;
    }
    cout<<line[index]<<endl;
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

vector<string> SourceProgram::getWords(string parser)
{
    vector<string> line;
    string word;
    stringstream strem(parser);
    while(strem>>word)
    {
        line.push_back(word);
    }
    return line;
}


