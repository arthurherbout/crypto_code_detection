#include <cstdio>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <sstream>
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

#define debug(x) cerr<<#x<<" = "<<(x)<<endl

typedef long long ll;
ll N, X, s[1000], e[1000], t[1000], l[1000];
int C;

ll tryenter(ll starttime, ll startpoint)
{
	ll exittime=X;
	for(int i=0; i<C; i++)
	{
		ll ss=s[i], tt=t[i];
		if(tt<starttime){
			ss+=starttime-tt;
			ss%=N;
			tt=starttime;
		}
		while(tt<=t[i]+l[i])
		{
			ll at=(((startpoint-(tt-starttime))%N)+N)%N;
			if(ss==at)
				exittime=min(exittime, tt-1);
			if(tt<t[i]+l[i] && ((ss+1)%N)==at)
				exittime=min(exittime, tt);
			tt++;
			ss++;
			ss%=N;
		}
		/*
		ll pt=(starttime+startpoint)%N;
		ll ss=s[i], tt=t[i];
		if(tt<starttime){
			ss+=starttime-tt;
			tt=starttime;
		}
		ll cs=(ss+tt)%N;
		if(cs>pt)
			pt+=N;
		ll collide=(pt-cs+1)/2;
		if(tt+collide <= t[i]+e[i]-s[i])
		{
			exittime=min(exittime, tt+collide-1);
		}
		*/
	}
	return max(0ll, exittime-starttime);
}

int main()
{
	int _T;
	string line;
	getline(cin, line);
	istringstream(line)>>_T;
	for(int _t=1; _t<=_T; _t++)
	{
		scanf("%d%lld%lld", &C, &X, &N);
		for(int i=0; i<C; i++){
			scanf("%lld%lld%lld", s+i, e+i, t+i);
			s[i]--;
			e[i]--;
			l[i]=(e[i]-s[i]+N)%N;
			assert(l[i]>0);
		}
		ll res=0;
		

		for(int j=0; j<N; j++)
		for(int i=0; i<=X; i++){
			res=max(res, tryenter(i, j));
		}
		printf("Case #%d: %lld\n", _t, res);
	}
}
