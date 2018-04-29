#include<bits/stdc++.h>
#include<SourceProgram.h>
#include<SourceLine.h>
#include<OpCodeTable.h>
#include<SyntaxValidator.h>
#include<AssemblyListing.h>
#include<SymbolTable.h>
#include<ValidatorUtilities.h>


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
    while(getline(cin,parser))
    {
        string word;
        line.clear();
        stringstream strem(parser);


        line = getWords(parser);
//   We Collect all words in vector called line


        SourceLine sourceLine;
        if(!isComment(line))
        {
            sourceLine = identifier(line, parser);
            updateLocationCounter(sourceLine);
        }
        else  // Comment Line
        {
            string comment = "";
            int index = 0;
            while(index != line.size())
            {
                comment += line[index++] + " ";
            }
            sourceLine.setComment(comment);
            sourceLine.setLable("");
            sourceLine.setOperand("");
            sourceLine.setOperation("");
            write(sourceLine,"");
        }
        lineNumber++;
    }

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
    OpInfo opinfo = opCodeTable->getInfo(upperForm);
    if(opinfo.getOpCode() == "11" && direcive.count(upperForm) == 0)
    {
        cout<<line[index]<<"\n";
        if(getUpper(line[index]) != "END")
        sourceLine.setLable(line[index++]);
        else
            sourceLine.setOperation(line[index++]);
    }
    if(index != line.size()){
        sourceLine.setOperation(line[index++]);
     if(getUpper(sourceLine.getOperation())=="BYTE"){
        if(index != line.size()) {
        if(line[index][0]=='C')
          return handleByte(sourceLine, line, index, parser);
     }
     }

    }
    if(index != line.size())
        sourceLine.setOperand(line[index++]);

    string comment = "";
    while(index != line.size())
    {
        comment += line[index++]+" ";
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
    string locationCounterinhex = stream.str();
    assemblyListing.write(sourceLine, locationCounterinhex, error);
}
void SourceProgram::detectStart(SourceLine sourceLine)
{

    SyntaxValidator syntaxValidator;
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

void SourceProgram::updateLocationCounter(SourceLine sourceLine)
{
   string error = "";
    SyntaxValidator syntaxValidator;
    if(syntaxValidator.isValid(sourceLine))
    {
        if(getUpper(sourceLine.getOperation()) == "START")
        {
            start++;
            if(start == 1 && lineNumber == 0){
                detectStart(sourceLine);
            }
            else
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
        write(sourceLine, error);

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

               int numberOfWords = ValidatorUtilities::split(sourceLine.getOperand(), ',').size();
                locationCounter=locationCounter + 3 * numberOfWords;
            }
            else if(operation == "BYTE") {
                if(sourceLine.getOperand()[0]=='C')
                  locationCounter = locationCounter + (sourceLine.getOperand().length()) - 3;
                else
                  locationCounter = locationCounter + ((sourceLine.getOperand().length() - 3 + 1 )/2);
            }
            else if(operation == "RESB") {
             locationCounter = locationCounter + stoi(sourceLine.getOperand());

            }
            else if(operation == "RESW") {
             locationCounter = locationCounter + 3 * stoi(sourceLine.getOperand());
            }
        }
    }
    else
    {
        write(sourceLine, syntaxValidator.getErrorMessage());

    }
}
SourceLine SourceProgram::handleByte(SourceLine sourceLine, vector<string> line, int index, string subject){
    string operand = "";
    string comment="";
    std::regex pattern("C'.*'|\\w+");
    for (auto i = std::sregex_iterator(subject.begin(), subject.end(), pattern); i != std::sregex_iterator(); ++i) {
   // cout<<i->str()<<"\n";
     if(i->str()[0] == 'C') {
        operand = i->str();
        i++;
        for (; i != std::sregex_iterator(); i++)
          comment += i->str()+" ";

        break;

        }
    }
sourceLine.setOperand(operand);
sourceLine.setComment(comment);
return sourceLine;
}
