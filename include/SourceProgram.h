#include <bits/stdc++.h>
#include<SourceLine.h>
#include<SymbolTable.h>
#include<AssemblyListing.h>
#ifndef SOURCEPROGRAM_H
#define SOURCEPROGRAM_H

using namespace std;

class SourceProgram
{
    public:
        SourceProgram();
        vector<SourceLine> parse(char* fileName);
        static string getUpper(string word);

    private:

        string getComment(int index, vector<string> line);
        SourceLine identifier(vector<string> sourceLine, string psrser);
        bool isComment(vector<string> sourceLine);
        vector<string> getWords(string parser);
        SourceLine handleSpacesInOperand(SourceLine sourceLine, string parser, string pattern, char beginCharacter);
        map<string, bool> getLiteralTable();

        void write(SourceLine sourceLine, string error);
        void detectStart(SourceLine sourceLine);
        void updateLocationCounter(SourceLine sourceLine);
        void makeLiteralPool();

        int start = 0, lineNumber = 0, locationCounter = 0;
        AssemblyListing assemblyListing;
        SymbolTable* symbolTable;
        set<string> direcive;
        vector<SourceLine> sourcelines;
        map <string, bool> lieralTable;
};

#endif
