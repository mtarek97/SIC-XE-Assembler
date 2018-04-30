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
    direcive.insert("START");
    direcive.insert("END");
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
            lineNumber++;
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
    OpInfo opinfo =opCodeTable->getInfo(upperForm);
    string s= upperForm;
    s.erase(s.begin());
   // cout<<s;
    if(opinfo.getOpCode() == opinfo.NOT_FOUND && direcive.find(upperForm) == direcive.end() &&opCodeTable->getInfo(s).getOpCode() == opinfo.NOT_FOUND)
    {
       // cout<<line[index]<<line[index].length()<<"\n";
        sourceLine.setLable(line[index++]);
    }
    if(index != line.size()){
        sourceLine.setOperation(line[index++]);
     if(getUpper(sourceLine.getOperation())=="BYTE"){
        if(index != line.size()) {
        if(line[index][0]=='C' || line[index][0]=='c')
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
                return;
            }
            else
            error = "You wirte START operation more than one time";
        }

        if(sourceLine.getLable() != "")
        {
            if(symbolTable.hashtable.count(sourceLine.getLable()) != 0)
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
        }
        else if(sourceLine.getOperation()[0] == '+')
        {
        string withOutPlus = sourceLine.getOperation();
        withOutPlus.erase(withOutPlus.begin());
        if(opCodeTable->getInfo(getUpper(withOutPlus)).getOpCode() != opinfo.NOT_FOUND){
                locationCounter+=4;
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
                if(sourceLine.getOperand()[0]=='C' || sourceLine.getOperand()[0]=='c')
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
    std::regex pattern("C'.*'|c'.*'|\\w+");
   // cout<<subject;
    int flag = 0, counter = 0;
    for (auto i = std::sregex_iterator(subject.begin(), subject.end(), pattern); i != std::sregex_iterator(); ++i) {
     cout<<i->str()<<"\n";

     if(flag){
        comment += i->str()+" ";
        continue;
     }
     if((i->str()[0] == 'C' || i->str()[0] == 'c') && counter) {
        operand = i->str();
        flag = 1;
        continue;
     }
     counter++;
    }
sourceLine.setOperand(operand);
sourceLine.setComment(comment);
//cout<<comment;
return sourceLine;
}
