#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <cmath>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <queue>
#include <memory.h>
#include <stack>
#define mp make_pair
#define pb push_back                     
#define int64 long long
#define ld long double  
#define setval(a,v) memset(a,v,sizeof(a))
using namespace std;


void PreCalc(){
}

const int maxn=3000000;
const int maxn2=maxn/2;

int a[maxn];
set<int> use;


void solve(){
	int n;
	cin>>n;
	
	memset(a,0,sizeof(a));
	
	use.clear();
	
	for (int i=0;i<n;i++){
		int p,c;
		cin>>p>>c;
		p+=maxn2;
		a[p]=c;
		use.insert(p);
	}
	
	int ans=0;

	for (;;){
		bool q=false;
		vector<int> tmp(use.begin(),use.end());
		for (int j=0;j<tmp.size();j++){
			int i=tmp[j];
			if (a[i]>=2)	
			{
				a[i-1]++;
				a[i+1]++;
				a[i]-=2;
				use.insert(i-1);
				use.insert(i+1);							
				ans++;
				q=true;
			}
			if (a[i]==0)
				use.erase(i);
		}
		if (!q)
		 break;
	}
	cout<<ans<<endl;
}

int main()
{
	PreCalc();
	int t;
	cin>>t;
	for (int i=1;i<=t;i++)
		{
			printf("Case #%d: ",i);
			solve();
			if (i%5==0)
				cerr<<"SOLVED:"<<i<<endl;
		}


  return 0;
}