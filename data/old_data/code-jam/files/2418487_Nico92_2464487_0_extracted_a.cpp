#include <iostream>
#include <string>
#include <math.h>

#include <stdlib.h>
#include <stdio.h>

#define REP(i,n) for(int i=0;i<(n);i++)
#define FOR(v,p,k) for(int v=p;v<=k;v++)

using namespace std;
typedef long long ll;

int main()
{
	int T;
	ll r,t,y;
	long double delta;
	cin>>T;
	FOR(c,1,T)
	{
		cin>>r;
		cin>>t;
		delta=(2*r-1)*(2*r-1)+8*t;
		y=floor((1-2*r+sqrt(delta))/4+0.0000001);
		printf("Case #%d: %lld\n",c,y);
	}
}
