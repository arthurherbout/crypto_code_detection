#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <queue>
#include <set>
#include <map>
#include <algorithm>
#define f(x,y) for(int x=0;x<y;x++)
#define pb push_back
#define mp make_pair
#define All(x) x.begin(),x.end()
using namespace std;

typedef long long int LL;
typedef vector<int> vi;

int main()
{
	ifstream fin("b-small.in");
	ofstream fout("b-small.out");
	
	int T,L,P,C;
	fin>>T;
	f(i,T)
	{
        fin>>L>>P>>C;
        cout<<L<<" "<<P<<" "<<C<<endl;
        int ind=0;
        
        if(L*C>=P)
        {
            cout<<"Case #"<<i+1<<": "<<0<<endl;
            fout<<"Case #"<<i+1<<": "<<0<<endl;
        }
        else
        {
            double d=log(double(P)/double(L))/log(C);
            cout<<"Case #"<<i+1<<": "<<ceil(log(d)/log(2))<<endl;
            fout<<"Case #"<<i+1<<": "<<ceil(log(d)/log(2))<<endl;
        }
    }
	
    cout<<"End."<<endl;
	cin.get();
	return 0;
}
