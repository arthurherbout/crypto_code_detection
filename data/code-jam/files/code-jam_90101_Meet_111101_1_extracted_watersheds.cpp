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
    int num=0;
    ifstream fin;
    fin.open("input.txt");
    ofstream fout;
    int count=1;
    fout.open("output.txt");
    fin>>num;
    while(num>0)
    {     
         int H,W;
         fin>>H>>W;

         cout<<H<<" "<<W;
         vector <vector<int> > mp(H,vector<int>(W));
         vector <vector<bool> > isb(H,vector<bool>(W,false));
         vector <vector<bool> > isd(H,vector<bool>(W,false)); 
         vector <vector<char> > ans(H,vector<char>(W,false));        
         int cnt=0;
         for(int i=0;i<H;i++)
         for(int j=0;j<W;j++)
         {
                 fin>>mp[i][j];
         }
         int curi=0,curj=0;
         
         for(int i=0;i<H;i++)
         for(int j=0;j<W;j++)
         {
                 //cout<<"Enter";
         if(isd[i][j])continue;
         curi=i;curj=j;
         bool found=false;
         vector <int> sx;
         vector <int> sy;
         while(!found)
         {
                      if(isd[curi][curj])
                      {
                            for(int k=sx.size()-1;k>=0;k--)
                              {
                              ans[sx[k]][sy[k]]=ans[curi][curj];
                              isd[sx[k]][sy[k]]=true;
                              }
                              sx.clear();sy.clear();
                              found=true; 
                      }
             vector <int> cmparr;
             if(curi-1>=0)cmparr.push_back(mp[curi-1][curj]);else cmparr.push_back(99999);
             if(curj-1>=0)cmparr.push_back(mp[curi][curj-1]);else cmparr.push_back(99999);
             if(curj+1<W)cmparr.push_back(mp[curi][curj+1]);else cmparr.push_back(99999);
             if(curi+1<H)cmparr.push_back(mp[curi+1][curj]);else cmparr.push_back(99999);
             for(int l=0;l<cmparr.size();l++)cout<<cmparr[l]<<" ";
             
             int p=0;
             for(int l=0;l<cmparr.size();l++)if(cmparr[l]!=99999){p=1;break;}
             if(p==0)
             {
                  //cout<<mp[curi][curj]<<" single ele at position "<<curi<<" "<<curj<<endl;
                  found=true;
                  isb[curi][curj]=true;
                  isd[curi][curj]=true;
                  ans[curi][curj]=cnt + 'a';
                  cnt++;
             }
             vector <int> :: iterator mine=min_element(cmparr.begin(),cmparr.end());
             //cout<<*mine <<" is the min element and cur is "<<mp[curi][curj]<<" ";
             //_getch();
             if(mp[curi][curj]>*mine)
             {
             sx.push_back(curi);
             sy.push_back(curj);
             int k=-1;
              if(curi-1>=0 && mp[curi-1][curj]==*mine)curi--;
              else if(curj-1>=0 && mp[curi][curj-1]==*mine)curj--;
              else if(curj+1<W && mp[curi][curj+1]==*mine)curj++;
              else if(curi+1<H && mp[curi+1][curj]==*mine)curi++;
              
              else {fout<<"error in min element";cout<<"error in min element";}
             }
             else if(mp[curi][curj]<=*mine)
             {
                  //cout<<mp[curi][curj]<<" at position "<<curi<<" "<<curj<<endl;
                  found=true;
                  if(!isd[curi][curj])
                  {
                  isb[curi][curj]=true;
                  isd[curi][curj]=true;
                  ans[curi][curj]=cnt + 'a';
                                    cnt++;
                  }
                  for(int k=sx.size()-1;k>=0;k--)
                  {
                  ans[sx[k]][sy[k]]=ans[curi][curj];
                  isd[sx[k]][sy[k]]=true;
                  }
                  sx.clear();sy.clear();
             }
             cout<<endl;
             
         }
         }
         for(int i=0;i<H;i++)
         {
         for(int j=0;j<W;j++)
         {
                 cout<<ans[i][j]<<" ";
                 }
                 cout<<endl;
                 }
          fout<<"Case #"<<count<<": "<<endl;
          for(int i=0;i<H;i++)
          {
          for(int j=0;j<W;j++)
          {
                 fout<<ans[i][j]<<" ";
                 }
                 fout<<endl;
                 }
         
          count++;                  
          num--;
          //_getch();
    }
    cout<<"done";
    _getch();
}

