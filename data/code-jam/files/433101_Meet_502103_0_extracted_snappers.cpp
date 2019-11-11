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
          int nsnappers;
          long long int ntimes;
          fin>>nsnappers>>ntimes;
        vector <bool> cables(nsnappers+1,0);
        cables[0]=true;
        vector <bool> snappers(nsnappers,0);
        
        for(long long i=0;i<ntimes;i++)
        {
                for(int j=0;j<nsnappers;j++)
                        if(cables[j])snappers[j]=!snappers[j];
                        
                for(int j=0;j<nsnappers;j++)
                        if(cables[j] && snappers[j])cables[j+1]=true;
                        else cables[j+1]=false;
                
                }
                string ans;
          if(cables[nsnappers])ans="ON";
          else ans="OFF";    
          fout<<"Case #"<<count<<": "<<ans<<endl;
          count++;                  
          num--;
          //_getch();
    }
    cout<<"done";
    _getch();
}

