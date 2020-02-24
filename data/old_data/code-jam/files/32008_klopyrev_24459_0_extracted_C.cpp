#include <cstdio>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <algorithm>
using namespace std;
#define FR(i,a,n) for(int (i) = (a); (i) < (n); (i)++)
#define FOR(i,n) FR(i,0,n)
#define RF(i,a,n) for(int (i) = int(n)-1; (i) >= (a); (i)--)
#define ROF(i,n) RF(i,0,n)
typedef long double ld;

ld pr[30][4];
int cur[30];
int Q;

ld calcProb()
{
	ld res = 1;
	FOR(i,Q)
		res*=pr[i][cur[i]];
	return res;
}
int main()
{
	int TESTS;
	scanf("%d", &TESTS);
	FOR(tests,TESTS)
	{
		int M;
		scanf("%d%d", &M, &Q);
		long long dif = 1<<Q;
		dif*=dif;
		FOR(i,Q) FOR(p,4)
			scanf("%Lf", pr[i]+p);
		if(dif<=M)
		{
			printf("Case #%d: 1.0\n", tests+1);
			continue;
		}
		FOR(i,Q)
			sort(pr[i], pr[i]+4);
		vector<ld> best;
		FOR(i,1<<(2*Q))
		{
			FOR(j,Q)
			{
				cur[j] = i>>(j*2)&3;
			}
			best.push_back(calcProb());
		}
		sort(best.begin(), best.end());
		ld res = 0;
		FOR(i,M)
			res+=best[best.size()-1-i];
		printf("Case #%d: %.9Lf\n", tests+1, res);
	}
}
