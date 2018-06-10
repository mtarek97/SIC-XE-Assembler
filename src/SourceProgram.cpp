#include<bits/stdc++.h>
#include<SourceProgram.h>
#include<SourceLine.h>
#include<OpCodeTable.h>
#include<SyntaxValidator.h>
#include<AssemblyListing.h>
#include<SymbolTable.h>
#include<ValidatorUtilities.h>
#include<UpdateLocationCounter.h>
#include <ObjectCodeGenerator.h>
#include "DetectLiterals.h"
#include "LiteralTable.h"
SourceProgram::SourceProgram()
{
    this->symbolTable = SymbolTable::getSymbolTable();
}
vector<SourceLine> SourceProgram::parse(char* fileName)
{
    freopen(fileName, "r", stdin);
    direcive.insert("START");
    direcive.insert("END");
    direcive.insert("BYTE");
    direcive.insert("RESB");
    direcive.insert("RESW");
    direcive.insert("WORD");
    direcive.insert("LTORG");
    direcive.insert("ORG");
    direcive.insert("EQU");

    string parser, word;
    bool stop = false;
    this->locationCounter=0;
    vector<string> line;
    while(getline(cin,parser))
    {
        string word;
        line.clear();
        stringstream strem(parser);
        line = getWords(parser);
        if(line.size() == 0)
            continue;
//   We Collect all words in vector called line


        SourceLine sourceLine;

        sourceLine.setLocationCounter(locationCounter);
        if(sourcelines.size() != 0)
            sourcelines[sourcelines.size() - 1].setNextInstruction(locationCounter);


        if(!isComment(line))
        {
            sourceLine = identifier(line, parser);
            updateLocationCounter(sourceLine);
            lineNumber++;
        if(sourceLine.getOperand()[0] == '=' )
        {
            if(!lieralTable.count(sourceLine.getOperand()))
             this->lieralTable[sourceLine.getOperand()] = make_pair(false, 0);
        }
        if(getUpper(sourceLine.getOperation()) == "LTORG")
        {
            makeLiteralPool();
        }
        }
        else
        {
            sourceLine.setComment(getComment(0, line));
            sourceLine.setLable("");
            sourceLine.setOperand("");
            sourceLine.setOperation("");
            write(sourceLine,"");
        }
        this->sourcelines.push_back(sourceLine);


    }

makeLiteralPool();
(LiteralTable::getLiteralsTable())->SetLiteralsTable(lieralTable);
sourcelines[sourcelines.size() - 1].setNextInstruction(locationCounter);
return (sourcelines);

}
std::map<std::string,std::pair< bool, int> > SourceProgram::getLiteralTable(){
return lieralTable;
}

void SourceProgram::makeLiteralPool()
{
    vector<SourceLine> newLines = DetectLiterals::detect(this->locationCounter, lieralTable);

    for(int i = 0; i < newLines.size(); i++)
    {
        sourcelines.push_back(newLines[i]);
        if(sourcelines.size() != 0)
        {
            sourcelines[sourcelines.size() - 1].setNextInstruction(locationCounter);
        }
        locationCounter = newLines[i].getLocationCounter();
        lieralTable[newLines[i].getOperand()] = make_pair(true, locationCounter);
        cout<<lieralTable[newLines[i].getOperand()].first<<" "<< lieralTable[newLines[i].getOperand()].second<<"\n";
        write(newLines[i], "");
    }
    if(newLines.size() != 0)
      locationCounter = UpdateLocationCounter::setLocationCounter(sourcelines[sourcelines.size() - 1].getLocationCounter(), sourcelines[sourcelines.size() - 1]).first;

}
string SourceProgram::getUpper(string word)
{
    string upperForm =  word;
    std::transform(upperForm.begin(), upperForm.end(), upperForm.begin(), ::toupper);
    return upperForm;

}
SourceLine SourceProgram::identifier(vector<string> line, string parser)
{

    int index = 0;
    SourceLine sourceLine;
    OpCodeTable* opCodeTable = OpCodeTable::getOpTable();

    string upperForm =  getUpper(line[index]);
    OpInfo opinfo =opCodeTable->getInfo(upperForm);
    string withoutSign= upperForm;
    withoutSign.erase(withoutSign.begin());
    if(opinfo.getOpCode() == opinfo.NOT_FOUND && direcive.find(upperForm) == direcive.end() &&opCodeTable->getInfo(withoutSign).getOpCode() == opinfo.NOT_FOUND)
    {
        sourceLine.setLable(line[index++]);
    }
    if(index != line.size())
    {
        sourceLine.setOperation(line[index++]);
        if(getUpper(sourceLine.getOperation())=="BYTE")
        {
            if(index != line.size())
            {
                if(line[index][0]=='C' || line[index][0]=='c')
                    return handleSpacesInOperand(sourceLine, parser, "C'.*'|c'.*'|\\w+", 'C');
            }
        }
        else if(index != line.size() && line[index][0]=='=' && toupper(line[index][1])=='C')
            return handleSpacesInOperand(sourceLine, parser, "=C'.*'|=c'.*'|\\w+", '=');

    }
    if(index != line.size())
        sourceLine.setOperand(line[index++]);


    sourceLine.setComment(getComment(index, line));

    return sourceLine;
}

string SourceProgram::getComment(int index, vector<string> line)
{
    string comment = "";
    while(index != line.size())
    {
        comment += line[index++]+" ";
    }
    return comment;
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
void SourceProgram::write(SourceLine sourceLine, string error)
{
    std::stringstream stream;
    stream << std::hex << locationCounter;
    string locationCounterinhex = stream.str();
    assemblyListing.write(sourceLine, locationCounterinhex, error);
}
void SourceProgram::updateLocationCounter(SourceLine sourceLine)
{
    string error = "";
    SyntaxValidator syntaxValidator;
    if(syntaxValidator.isValid(&sourceLine))
    {
        if(getUpper(sourceLine.getOperation()) == "START")
        {
            start++;
            if(start == 1 && lineNumber == 0)
            {
                locationCounter = UpdateLocationCounter::detectStart(locationCounter, sourceLine);
                if(sourceLine.getLable() != "")
                    symbolTable->insert(sourceLine.getLable(), locationCounter);
                write(sourceLine, "");
                return;
            }
            else
            {
                error = "You wirte START operation more than one time";
                sourceLine.setIsValid(false);
            }
        }

        if(sourceLine.getLable() != "")
        {
            if(symbolTable->hashtable.count(sourceLine.getLable()) != 0)
            {
                error = "This lable is used before";
                sourceLine.setIsValid(false);
            }
            else
                symbolTable->insert(sourceLine.getLable(), locationCounter);
        }

        write(sourceLine, error);
        error = "";
        pair<int,string> result;
        result = UpdateLocationCounter::setLocationCounter(locationCounter, sourceLine);
        locationCounter = result.first;
        error = result.second;
        if(error != ""){
        SourceLine emptyy;
        write(emptyy, error);
        }
    //    ObjectCodeGenerator* generator = ObjectCodeGenerator::getObjectCodeGenerator(); // testing purposes !!
  //      generator->setSymbolTable(symbolTable);

    }
    else
    {
        write(sourceLine, sourceLine.getErrorMessage());
        sourceLine.setIsValid(false);
    }
}
SourceLine SourceProgram::handleSpacesInOperand(SourceLine sourceLine, string subject, string patern, char beginCharacter)
{
    string operand = "";
    string comment="";
    std::regex pattern(patern);
    int flag = 0, counter = 0;
    for (auto i = std::sregex_iterator(subject.begin(), subject.end(), pattern); i != std::sregex_iterator(); ++i)
    {
        if(flag)
        {
            comment += i->str()+" ";
            continue;
        }
        if((toupper(i->str()[0]) == beginCharacter) && counter)
        {
            operand = i->str();
            flag = 1;
            continue;
        }
        counter++;
    }
    sourceLine.setOperand(operand);
    sourceLine.setComment(comment);
    return sourceLine;
}
