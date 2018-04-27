#include<bits/stdc++.h>
#include "AssemblyListing.h"
#include<SourceLine.h>
AssemblyListing::AssemblyListing()
{
    //ctor
}
void AssemblyListing::write(SourceLine sourceLine, string locationCounter, string error){
if(sourceLine.getOperation() == ""){
    cout<<sourceLine.getComment()<<endl;
}
else {
    cout<<sourceLine.getLable()<<"  "<<sourceLine.getOperation()<<" "<<sourceLine.getOperand()<<"   "<<sourceLine.getComment()<<"   "<<locationCounter<<endl;
    if(error != ""){
        cout<<error<<endl;
    }
}
}



