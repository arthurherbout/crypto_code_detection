#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <fstream>

#include <conio.h>
using namespace std;
bool allzero(string s)
{
     if(s.length()==1)return true;
     for(int i=1;i<s.length();i++)
     {
     if(s[i]!='0')return false;
     }
     return true;
 }
int main()
{
    int num=0;
    ifstream fin;
    fin.open("input.txt");
    ofstream fout;
    int count=1;
    fout.open("output.txt");
    fin>>num;
    while(num>0)
    {     
          int n;
        fin>>n;
        stringstream ss;
        ss<<n;
        string s;
        ss>>s;
        vector<bool> flag(s.length(),false);
        if(s[0]!='0' && allzero(s))
        {
                     fout<<"Case #"<<count<<": "<<n*10<<endl;
          count++;                  
          num--;
          continue;
          }
        int i=0;
        for(i=n+1;i<=10000000;i++)
        {
        cout<<"checking for "<<i<<endl;
                stringstream s1;
                s1<<i;
                string c;
                s1>>c;
                int q=0;    
                for(int j=0;j<c.length();j++)
                {
                int p=0;
                
                for(int k=0;k<s.length();k++)
                {
                        if(flag[k]==false && s[k]==c[j])
                        {
                                          //cout<<"matched for "<<c[j]<<" ";
                                          p=1;
                                          flag[k]=true;
                                          break;
                        }
                }
                if(c[j]=='0')p=1
                ;       
                if(p==0)break;
                if(p==1 && j==c.length()-1)
                    {
                                            q=1;
                        for(int k=0;k<flag.size();k++)
                        {if(flag[k]==false)
                        {
                        //cout<<"not for "<<s[k];
                        q=0;}}

                    break;
                    }
                }
                for(int k=0;k<s.length();k++)flag[k]=false;
                if(q==1)break;
                //_getch();
        }
          fout<<"Case #"<<count<<": "<<i<<endl;
          count++;                  
          num--;
          //_getch();
    }
    cout<<"done";
    _getch();
}

