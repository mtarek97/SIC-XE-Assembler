#include<bits/stdc++.h>
#include<SourceProgram.h>
#include<SourceLine.h>
#include<OpCodeTable.h>
#include<SyntaxValidator.h>
#include<AssemblyListing.h>
#include<SymbolTable.h>


SourceProgram::SourceProgram()
{
}
void SourceProgram::parse(char* fileName)
{
    freopen(fileName, "r", stdin);
    direcive.insert("BYTE");
    direcive.insert("RESB");
    direcive.insert("RESW");
    direcive.insert("WORD");

    string parser, word;
    bool stop = false;
    this->locationCounter=0;
    vector<string> line;
    SourceLine sourceLine;
    getline(cin, parser);
    line = getWords(parser);
    sourceLine = identifier(line);
    detectStart(sourceLine);
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
        if(!isComment(line))
        {
            sourceLine = identifier(line);
            updateLocationCounter(sourceLine);
        }
        else  // Comment Line
        {
            string comment = "";
            int index = 0;
            while(index != line.size())
            {
                comment += line[index++];
            }
            sourceLine.setComment(comment);
            sourceLine.setLable("");
            sourceLine.setOperand("");
            sourceLine.setOperation("");
            write(sourceLine,"");
        }
    }

}
string SourceProgram::getUpper(string word)
{
    string upperForm =  word;
    std::transform(upperForm.begin(), upperForm.end(), upperForm.begin(), ::toupper);
    return upperForm;

}
SourceLine SourceProgram::identifier(vector<string> line)
{

    int index = 0;
    SourceLine sourceLine;
    OpCodeTable* opCodeTable = OpCodeTable::getOpTable();
    string upperForm =  getUpper(line[index]);
    OpInfo opinfo = opCodeTable->getInfo(upperForm);
    if(opinfo.getOpCode() == "11" && direcive.count(upperForm) == 0)
    {
        sourceLine.setLable(line[index]);
        index++;
    }
    sourceLine.setOperation(line[index++]);
    if(index != line.size())
        sourceLine.setOperand(line[index++]);

    string comment = "";
    while(index != line.size())
    {
        comment += line[index++];
    }
    sourceLine.setComment(comment);

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
void SourceProgram::write(SourceLine sourceLine, string error)
{
    std::stringstream stream;
    stream << std::hex << locationCounter;
    AssemblyListing assemblyListing;
    string locationCounterinhex = stream.str();
    assemblyListing.write(sourceLine, locationCounterinhex, error);
}
void SourceProgram::detectStart(SourceLine sourceLine)
{

    SyntaxValidator syntaxValidator;
    if(getUpper(sourceLine.getOperation()) == "START")
    {
        if(syntaxValidator.isValid(sourceLine))
        {
            this->locationCounter = std::stoi(sourceLine.getOperand(), nullptr, 2);
            write(sourceLine, "");

        }
        else
        {
            write(sourceLine, syntaxValidator.getErrorMessage());
        }
    }
    else
    {
        updateLocationCounter(sourceLine);
    }
}

void SourceProgram::updateLocationCounter(SourceLine sourceLine)
{
   string error = "";
    SyntaxValidator syntaxValidator;
    if(syntaxValidator.isValid(sourceLine))
    {
        if(getUpper(sourceLine.getOperation()) == "START")
        {
            error = "You wirte START operation more than one time";
        }

        if(sourceLine.getLable() != "")
        {
            if(symbolTable.search(sourceLine.getLable()) != -1)
            {
                error = "This lable is used before";
            }
            else
                symbolTable.insert(sourceLine.getLable(), locationCounter);

        }
        OpCodeTable* opCodeTable = OpCodeTable::getOpTable();
        OpInfo opinfo = opCodeTable->getInfo(getUpper(sourceLine.getOperation()));

        if(opinfo.getOpCode() != "11")
        {
            locationCounter+=opinfo.getFormateBytes();

            if(sourceLine.getOperation()[0] == '+')
            {
                locationCounter++;
            }
        }
        else
        {
            string operation = getUpper(sourceLine.getOperation());
            if(operation == "WORD") {
                locationCounter+=3;
            }
            else if(operation == "BYTE") {
                locationCounter = locationCounter + (sourceLine.getOperand().length());
            }
            else if(operation == "RESB") {
             locationCounter = locationCounter + stoi(sourceLine.getOperand());

            }
            else if(operation == "RESW") {
             locationCounter = locationCounter + 3 * stoi(sourceLine.getOperand());
            }
        }
        write(sourceLine, error);
    }
    else
    {
        write(sourceLine, syntaxValidator.getErrorMessage());

    }
}
