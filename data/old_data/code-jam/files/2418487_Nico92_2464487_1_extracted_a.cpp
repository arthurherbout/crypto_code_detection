#include <iostream>
#include <string>
#include <math.h>

#include <stdlib.h>
#include <stdio.h>

#define REP(i,n) for(int i=0;i<(n);i++)
#define FOR(v,p,k) for(int v=p;v<=k;v++)
#define SQUARE(x) x*x

using namespace std;
typedef long long ll;

long double approx(long double x,long long r,long long t)
{
	return x-(2*x*x+x*(2*r-1)-t)/(4*x+(2*r-1));
}

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
		long double x=sqrt(t/2);
		long double y=0;
		while(fabs(x-y)>0.1)
		{
			x=y;
			y=approx(x,r,t);
		}
		long long var=floor(y);
		printf("Case #%d: %lld\n",c,var);
	}
}
