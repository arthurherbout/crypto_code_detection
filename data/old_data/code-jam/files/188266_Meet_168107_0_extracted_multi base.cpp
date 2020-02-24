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
 int fromDecimal(int n, int b)
{
   int result=0;
   int multiplier=1;
      
   while(n>0)
   {
      result+=n%b*multiplier;
      multiplier*=10;
      n/=b;
   }
      
   return result;
}
 int toDecimal(int n, int b)
{
   int result=0;
   int multiplier=1;
      
   while(n>0)
   {
      result+=n%10*multiplier;
      multiplier*=b;
      n/=10;
   }
      
   return result;
}
int sumofdigits(int n,int b)
{
     stringstream ss;
     ss<<n;
     string s;
     ss>>s;
     int ans=0;
     vector <int> stk;cout<<"came in:-"<<n<<" ";
 while(true){
                      ans=0;
for(int i=0;i<s.length();i++)
{
        ans+=int(pow(float(s[i]-'0'),float(2.0)));
        }
        cout<<"sum:-"<<ans;
        for(int j=0;j<stk.size();j++)if(stk[j]==ans)return ans;
        stk.push_back(ans);
        ans=fromDecimal(ans,b);
        
        cout<<"from decimal "<<ans<<endl;
        stringstream s1;
        s1<<ans;
        s1>>s;
        //_getch();
}
        return ans;
 }
int main()
{
    int num=0;
    ifstream fin;
    fin.open("input.txt");
    ofstream fout;
    int count=1;
    fout.open("output.txt");
                 char * buf = new char[19];      
    fin>>num;fin.getline(buf,19);
    while(num>0)
    {        
             int ans=2;
             char * buffer = new char[19];      
    fin.getline(buffer,19);
    string str=string(buffer);
    cout<<str;
    stringstream ss;ss<<str;
    int n;
    while(true)
    {
               int p=0;
                   stringstream ss;ss<<str;
         while(ss>>n)
        {
                    int d=fromDecimal(ans,n);
                    cout<<"next:-"<<n<<" ";
                    if(sumofdigits(d,n)!=1)
                    {
                    ans++;
                    cout<<"counting next"<<ans<<" "<<endl;
                    p=1;
                    break;
                    }
        }            
        if(p==0)break;
                    
                    }
          //cout<<buffer<<endl;
          fout<<"Case #"<<count<<": "<<ans<<endl;
                          count++;
         num--;
          _getch();
    }
    cout<<"done";
    _getch();
}

