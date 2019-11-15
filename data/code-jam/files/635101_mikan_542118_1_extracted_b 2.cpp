#include<cstdio>
#include<iostream>
#include<algorithm>
#include<cstring>
#include<cstdlib>
#include<string>
#include<vector>
#include<set>
#include<map>
#include<queue>
#include<bitset>
#include<gmpxx.h>

#define mp make_pair
#define pb push_back
#define repp(i,a,b) for(int i=(int)(a);i<=(int)(b);++i)
#define rep(i,n) repp(i,0,n-1)

typedef long long ll;
//typedef pair<int,int> pint;
typedef mpz_class BigInt;

using namespace std;

int x[100],v[100];

int main(){
	int c,n,k,b,t;
	cin>>c;
	rep(cas,c){
		cin>>n>>k>>b>>t;
		rep(i,n)cin>>x[i];
		rep(i,n)cin>>v[i];
		int ans=0;
		int now=0,hoge=0;
		if(!k){
			printf("Case #%d: 0\n",cas+1);
			continue;
		}
		for(int i=n-1;i>=0;--i){
			if((b-x[i])<=t*v[i]){
				ans+=hoge;
				++now;
				if(now==k)goto nex;
			}else{
				++hoge;
			}
		}
		cout<<"Case #"<<cas+1<<": IMPOSSIBLE"<<endl;
		continue;
nex:;
    		printf("Case #%d: %d\n",cas+1,ans);

	}

	return 0;
}
