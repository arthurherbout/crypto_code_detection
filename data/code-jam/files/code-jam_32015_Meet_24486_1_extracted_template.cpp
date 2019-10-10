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
    while(num>0)
    {
                int p,k,l;
                 fin>>p>>k>>l;
                 //vector< long int > Matrix(k*p, 0);
                 vector <long int> press(l);
                 int i=0;
                 for(i=0;i<l;i++)
                                 fin>>press[i];
                 sort(press.begin(),press.end(),greater<int>());
                 long long int ans=0;
                 int tk=1;
                 for(i=0;i<press.size();)
                 {
                 for(int j=0;j<k && i<press.size();j++,i++)
                         {
                                     ans+=(tk*press[i]);
                                     }
                                     
                                     tk++;
                                            }
                                            //cout<<ans;
                 fout<<"Case #"<<count<<": "<<ans<<endl;
                 num--;
                 count++;
                 
    }                 
    fin.close();
}
