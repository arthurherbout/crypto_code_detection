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

int main()
{
    int num=0,L,D;
    ifstream fin;
    fin.open("input.txt");
    ofstream fout;
    int count=1;
    fout.open("output.txt");
    fin>>L>>D>>num;
    int cnt=0;
    vector <string> dict(D);
    while(cnt<D)
    {
                fin>>dict[cnt];
                cnt++;
    }
    while(num>0)
    {     
          string whole;
          fin>>whole;
          vector <string> dm;
          for(int i=0;i<whole.length();i++)
          {
          string t="";
                  if(whole[i]=='(')
                  {i++;
                  while(whole[i]!=')')
                  {
                  t+=whole[i++];
                  }
                  }
                  else
                  t+=whole[i];
                  dm.push_back(t);
          }          
          //for(int i=0;i<dm.size();i++)cout<<dm[i]<<endl;
          int ans=0;
          for(int i=0;i<D;i++)
          {
                  int q=0;
                  //cout<<"finding match for "<<dict[i] <<" : ";
                  for(int j=0;j<L;j++)
                  {
                  int p=0;
                  //cout<<"Match for "<<dict[i][j]<<" and "<<dm[j];
                  
                  for(int k=0;k<dm[j].length();k++)
                  if(dict[i][j]==dm[j][k]){p=1;break;}
                  
                  if(p==0){//cout<<"Not Found"<<endl;
                  q=1;break;}
                  else {//cout<<"Found"<<endl;
                  }
                  }
                  if(q==0)ans++;
          }
          fout<<"Case #"<<count<<": "<<ans<<endl;
                          count++;
                  
                num--;
                //_getch();
                cout<<"==================="<<endl;
    }
    cout<<"done";
    _getch();
}
