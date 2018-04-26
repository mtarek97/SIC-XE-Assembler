#include <bits/stdc++.h>
#include<SourceLine.h>
#ifndef SOURCEPROGRAM_H
#define SOURCEPROGRAM_H

using namespace std;

class SourceProgram
{
    public:
        SourceProgram();
        void parse(char* fileName);
    private:
        SourceLine identifier(vector<string> sourceLine);
        int locationCounter=0;
        bool isComment(vector<string> sourceLine);
        vector<string> getWords(string parser);
};

#endif
