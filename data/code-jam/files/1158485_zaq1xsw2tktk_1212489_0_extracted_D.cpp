
#include <vector> 
#include <list> 
#include <map> 
#include <set> 
#include <deque> 
#include <queue> 
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
#include <cctype> 
#include <string> 
#include <cstring> 
#include <cstdio> 
#include <cmath> 
#include <cstdlib> 
#include <ctime> 
#include <fstream>
using namespace std; 
template<class T> T gcd(T a,T b){return a==0?b:gcd(b%a,a);}
template<class T> string tostring(T a){ostringstream os;os<<a;return os.str();}
int toint(string a){istringstream is(a);int p;is>>p;return p;}
long long toll(string a){istringstream is(a);long long p;is>>p;return p;}

#define mp make_pair
#define pb push_back

int p[10001];

int dx[4][2]={{0,0},{-1,1},{1,-1},{-1,1}};
int dy[4][2]={{-1,1},{0,0},{-1,1},{-1,1}};


void _main()
{
	string s;
	cin>>s;
	long long a=0,b=0;
	for(int i=0;i<s.size();i++)
	{a*=2;b*=2;
		if(s[i]!='?')a+=1;
		
		if(s[i]=='1')b++;
		
	}
	for(long long i=1;;i++)
	{
		long long l=i*i;
		if((l^b)&(a))continue;
		long long q=i*i;
		vector<int> v;
		while(q>0)
		{
			v.push_back(q%2);
			q/=2;
		}
		for(int j=0;j<v.size();j++)
		{
			cout<<v[v.size()-1-j];
		}
		cout<<endl;
		return;
	}
}

int main()
{
	freopen("in.txt","r",stdin);
	freopen("out.txt","w",stdout);
	int T;
	cin>>T;
	for(int i=1;i<=T;i++)
	{
		cout<<"Case #"<<i<<": ";
		_main();
		cerr<<i<<"end"<<endl;
	}
}