#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <queue>
#include <complex>
#include <cmath>
#include <map>
using namespace std;

#define REP(i,n) for(int i = 0; i < n; ++i)
#define FOR(i,a,b) for(int i = a; i <= b; ++i)
#define FORd(i,a,b) for(int i = a; i >= b; --i)
#define mset(ar,x) memset(ar,x,sizeof(ar))
#define pub push_back
#define fr first
#define sc second
#define mp make_pair

typedef long long llong;
int tc,n,pd,pg;

bool bisa(int d)
{
	int atas = pd*d;
	if(atas%100) return 0;
	int win = atas/100;
	
	int gwin = pg, gall = 100;
	while(gall < d) gall += 100, gwin += pg;
	if(gwin < win) return 0;
	int kalah = gall-gwin;
	if(kalah < d-win) return 0;
	return 1;
	
}

int main()
{
	scanf("%d",&tc);
	FOR(t,1,tc){
		scanf("%d %d %d",&n,&pd,&pg);
		bool yes = 0;
		FOR(i,1,n) if(bisa(i)){
			yes = 1;
			break;
		}
		
		if(yes) printf("Case #%d: Possible\n",t);
		else printf("Case #%d: Broken\n",t);
	}
	
	return 0;
}


