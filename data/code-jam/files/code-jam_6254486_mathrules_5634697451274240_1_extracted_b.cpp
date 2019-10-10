#include<iostream>
#include<conio.h>
#include<string.h>
#include<algorithm>
using namespace std;

int main() {
    int t,o=1;
    cin>>t;
    while(t--)
    {
        int counter=0,i=0,len;
        char inp[200];
        cin>>inp;
        len=strlen(inp);

        for(i=0;i<len-1;i++)
        {
            if(inp[i]!=inp[i+1])
             {   counter++;
                 //cout<<inp[i]<<"$"<<inp[i+1];
             }
        }
        if(inp[i]=='-')
                       counter++;
     	cout<<"Case #"<<o<<": "<<counter<<endl;
        
o++;
    }
     
    getch();
return 0;         
}
