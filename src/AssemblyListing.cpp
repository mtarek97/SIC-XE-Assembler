#include<bits/stdc++.h>
#include "AssemblyListing.h"
#include<SourceLine.h>
#include <string>
#include<fstream>
#include<iostream>
using namespace std;


AssemblyListing::AssemblyListing()
{
    ofstream out("ListingFile.txt",ios::out);
    std:: string line("Line    Loc            Source statement       \n");
    out << line;
    out.close();

}
void AssemblyListing::write(SourceLine sourceLine, string locationCounter, string error){
    std:: string line("");
    std:: string counterSring;
    counterSring = std::to_string(counter);
    int i;
    for(i = 0; i < 3-counterSring.length();i++){
        line = line +" ";
    }
    line = line + counterSring + "     ";



    if(sourceLine.getOperation() == ""){
        line = line +"         "+sourceLine.getComment()+"\n";
        //cout<<sourceLine.getComment()<<endl;
    } else {
        string zeros ="";
        if(locationCounter.length()<5)
        for(i = 0; i < 5 -locationCounter.length(); i++){
            zeros = zeros + "0";
        }
        locationCounter = zeros + locationCounter;
        line = line +locationCounter + "    "+sourceLine.getLable();
        for(i = 0; i < 20-sourceLine.getLable().length() && sourceLine.getLable().length() < 20;i++){
            line = line +" ";
        }

        line = line + sourceLine.getOperation();
        for(i = 0; i < 20-sourceLine.getOperation().length() && sourceLine.getOperation().length() < 20;i++){
            line = line +" ";
        }
        line = line + sourceLine.getOperand();
        for(i = 0; i < 20-sourceLine.getOperand().length() && sourceLine.getOperand().length() < 20;i++){
            line = line +" ";
        }
        line = line + sourceLine.getComment()+"\n";



      //  cout<<sourceLine.getLable()<<"  "<<sourceLine.getOperation()<<" "<<sourceLine.getOperand()<<"   "<<sourceLine.getComment()<<"   "<<locationCounter<<endl;
        if(error != ""){
            line = line +"       ****Error:"+ error +"\n";
            //cout<<error<<endl;
        }
    }
    ofstream out("ListingFile.txt",ios::out | ios::app);
    out << line;
    out.close();
    counter++;
}



