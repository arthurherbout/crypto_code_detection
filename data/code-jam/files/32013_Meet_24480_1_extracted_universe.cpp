#include <vector>
#include <fstream>
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
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <queue>
using namespace std;

int main()
{
    int num=0;
    int count=1;
    ifstream fin;
    fin.open("input.txt");
    ofstream fout;
    fout.open("output.txt");
    fin>>num;
//    cout<<num;
    while(num>0)
    {
                 int engine_num;
                 fin>>engine_num;//cout<<engine_num;
                 vector<string> engine_name(engine_num);
                 int i=0;
                 for(i=0;i<=engine_num;i++)
                 {
                 char temp[101];
                 fin.getline(temp,101);
                 engine_name.push_back(string(temp));
//                 cout<<temp<<endl;
                 }//cout<<"start";
/*                 for(i=0;i<engine_name.size();i++)
                 cout<<engine_name[i]<<"---"<<engine_name[i].length()<<endl;
                 cout<<"***************";
*/                 int querry_num;
                 fin>>querry_num;//cout<<querry_num;
                 vector<string> querry_name(querry_num);
                 for(i=0;i<=querry_num;i++)
                 {
                 char temp[101];
                 fin.getline(temp,101);
                 querry_name.push_back(string(temp));  
//                 cout<<temp<<endl;
                 }
/*                 for(i=0;i<querry_name.size();i++)
                 cout<<querry_name[i]<<"---"<<querry_name[i].length()<<endl;
                 cout<<"****************";
*/                 
                 list <string> name(engine_name.begin(),engine_name.end());
                 name.sort();
                 name.unique();
                 vector<string> track(name.begin(),name.end());
                 vector<bool> flag(name.size(),1);
                 int tot_swap=0;
                 int rem=-1;
                 int j=0;
                 for(i=0;i<querry_name.size();)
                         {    
                              
                              string t1=querry_name[i++];
                              for(int k=0;k<track.size();k++)
                              {//cout<<track[k]<<"-----------"<<t1<<endl;
                              if(track[k]==t1 )
                                             {
                                             //cout<<"Found match ---"<<t1<<endl;
                                             flag[k]=0;
                                             break;
                                             }
                              }
                              int count_true=0;                     
                              for(j=0;j<flag.size();j++)
                              {
                              if(track[j].length()!=0 && flag[j]==1)
                                            {rem=j;
                                           count_true++; 
                                           }
                              }
                              if(count_true==0)
                              {i--;
                             //cout<<"lucky string : "<<track[rem]<<endl;
                              tot_swap++;
                              for(j=0;j<flag.size();j++)
                              flag[j]=1;
                              }
                          }
                 //cout<<"Answer is:"<<tot_swap<<endl;
                 fout<<"Case #"<<count<<": "<<tot_swap<<endl;
                 num--;
                 count++;
    }                 
    fin.close();
}
