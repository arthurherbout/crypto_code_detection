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
#include<conio.h>
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
                int size=0;
                fin>>size;
                vector <int> v1(size);
                vector <int> v2(size);
                int i=0;
                for(i=0;i<size;i++)
                fin>>v1[i];
                for(i=0;i<size;i++)
                fin>>v2[i];
                sort(v1.begin(),v1.end());
                sort(v2.begin(),v2.end());
                list <int> l(v2.begin(),v2.end());
                l.reverse();
                vector <int> v3(l.begin(),l.end());

                long int ans=0;
                for(i=0;i<size;i++)
                ans+=v1[i]*v3[i];
                
                for(i=0;i<size;i++)
                cout<<v1[i];
                cout<<endl;
                for(i=0;i<size;i++)
                cout<<v3[i];
                fout<<"Case #"<<count<<": "<<ans<<endl;
                count++;
num--;
    }
    getch();
}
