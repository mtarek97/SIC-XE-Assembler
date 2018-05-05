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
        void parse(char* fileName);
        static string getUpper(string word);

    private:
        string getComment(int index, vector<string> line);
        SourceLine identifier(vector<string> sourceLine, string psrser);
        int locationCounter=0;
        bool isComment(vector<string> sourceLine);
        vector<string> getWords(string parser);
        set<string> direcive;
        void write(SourceLine sourceLine, string error);
        SymbolTable symbolTable;
        void detectStart(SourceLine sourceLine);
        void updateLocationCounter(SourceLine sourceLine);
        AssemblyListing assemblyListing;
        SourceLine handleByte(SourceLine sourceLine, vector<string> line, int index, string parser);
        int start = 0, lineNumber = 0;
};

#endif
