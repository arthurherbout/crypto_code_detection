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
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <fstream>
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
                int turn_time,a_trip,b_trip;
                fin>>turn_time>>a_trip>>b_trip;
                double real_time=double(turn_time)/60.00;
                vector <double> from_a;
                vector <double> to_b;
                vector <double> from_b;
                vector <double> to_a;
                //cout<<a_trip<<b_trip;
                //getch();
                int i=0;
                for(i=0;i<2*a_trip;i++)
                {
                string t;
                fin>>t;
                int temp1=int(t[0]-'0')*10 + (t[1]-'0');
                int temp2=int(t[3]-'0')*10 + (t[4]-'0');
                double ans=double(temp1)+double(temp2)/60.00;
                if(i%2==0)
                from_a.push_back(ans);
                else
                to_b.push_back(ans);
                }
                
                for(i=0;i<2*b_trip;i++)
                {
                string t;
                fin>>t;
                int temp1=int(t[0]-'0')*10 + (t[1]-'0');
                int temp2=int(t[3]-'0')*10 + (t[4]-'0');
                double ans=double(temp1)+double(temp2)/60.00;
                if(i%2==0)
                from_b.push_back(ans);
                else
                to_a.push_back(ans);
                }
                
                                
                for(i=0;i<from_a.size();i++)
                {
                for(int j=0;j<from_a.size()-i-1;j++)
                        {
                        if(from_a[j+1]<from_a[j])
                        {
                        swap(from_a[j+1],from_a[j]);
                        swap(to_b[j+1],to_b[j]);
                        }
                        }
                }
                
                //for(i=0;i<from_a.size();i++)
//                        cout<<from_a[i]<<" "<<to_b[i]<<endl;
                        
                for(i=0;i<from_b.size();i++)
                {
                for(int j=0;j<from_b.size()-i-1;j++)
                        {
                        if(from_b[j+1]<from_b[j])
                        {
                        swap(from_b[j+1],from_b[j]);
                        swap(to_a[j+1],to_a[j]);
                        }
                        }
                }
                //for(i=0;i<from_b.size();i++)
//                        cout<<from_b[i]<<" "<<to_a[i]<<endl;

                
                vector <char> sta;
                vector <double> from;
                vector <double> to;
                int a=0,b=0;
                for(i=0;i<(a_trip+b_trip);i++)
                {
                if(b==from_b.size() && a<from_a.size())
                {                               
                from.push_back(from_a[a]);
                to.push_back(to_b[a++]);
                sta.push_back('a');
                }
                else if(a==from_a.size() && b<from_b.size())
                {                               
                from.push_back(from_b[b]);
                to.push_back(to_a[b++]);
                sta.push_back('b');
                }
                else if(from_a[a]<=from_b[b] && a<from_a.size())
                {
                from.push_back(from_a[a]);
                to.push_back(to_b[a++]);
                sta.push_back('a');
                }
                else if(b<from_b.size())
                {
                from.push_back(from_b[b]);
                to.push_back(to_a[b++]);
                sta.push_back('b');
                }
                else
                    cout<<"ERROR";
                }
                
/*                for(i=0;i<from.size();i++)
                        cout<<from[i]<<" "<<to[i]<<" "<<sta[i]<<endl;
*/                
                

                int num_a=0,num_b=0,free_a=0,free_b=0;
                vector <bool> flag(from.size(),1);
                for(i=0;i<from.size();i++)
                {
                int p=0;
                for(int j=0;j<i;j++)
                        {
                                    //double te=to[j]+real_time;
                        //cout<<endl<<from[i]<<" "<<te<<" G ";
                        if(float(from[i])>=float((to[j]+real_time)) && flag[j]==1 && sta[i]!=sta[j])
                        {
                        //cout<<"IN";
                        p=1;
                        flag[j]=0;
                        break;
                        }
                        }
                if(p==1)
                        continue;
                else if(sta[i]=='a')
                     num_a++;
                else if(sta[i]=='b')
                    num_b++;
                else
                    cout<<"ERROR";
                    //cout<<num_a<<" "<<num_b<<endl;getch();
                }
//                int count_a=0,count_b=0;
//
//                while(1)
//                {
//                bool done=0;
//                if(count_a<from_a.size() && count_b<from_b.size() &&)
//                }
cout<<num_a<<"--------------"<<num_b<<endl;
fout<<"Case #"<<count<<": "<<num_a<<" "<<num_b<<endl;
                          count++;
                num--;
    }
}
