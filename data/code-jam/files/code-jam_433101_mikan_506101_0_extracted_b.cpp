#include<cstdio>
#include<iostream>
using namespace std;

typedef long long ll;

inline ll abs(ll a){
	return a>0?a:-a;
}

ll gcd(ll a,ll b){
	return b?gcd(b,a%b):a;
}

int main(){
	int c,n;
	ll t[1010];
	cin>>c;
	for(int cs=1;cs<=c;++cs){
		cin>> n;
		for(int i=0;i<n;++i)scanf("%lld",&t[i]);
		ll g=abs(t[1]-t[0]);
		for(int i=2;i<n;++i){
			g=gcd(g,abs(t[i]-t[0]));
		}
		ll ans=(g-(t[0]%g))%g;
		printf("Case #%d: %lld\n",cs,ans);
	}
	return 0;
}
