#include<iostream>
#include<algorithm>
#include<sstream>
#include<string>
#include<vector>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<cassert>
#include<numeric>
#include<set>
#include<map>
#include<queue>
#include<list>
#include<deque>
using namespace std;

#define FOR(i,a,b) for(int i = (a); i < (b); ++i)
#define REP(i,n) FOR(i,0,n)
#define FORE(it,x) for(typeof(x.begin()) it=x.begin();it!=x.end();++it)
#define pb push_back
#define all(x) (x).begin(),(x).end()
#define CLEAR(x,with) memset(x,with,sizeof(x))
#define sz size()
typedef long long ll;

int n, m;
bool small[10][10], large[10][10];

int main()
{
	int cases;
	cin >> cases;
	REP(cc,cases)
	{
		fprintf(stderr, "%d of %d ..\r", cc+1, cases);
		printf("Case #%d: ", cc+1);
		cin >> n;
		CLEAR(small,0); CLEAR(large,0);
		REP(i,n-1)
		{
			int a, b;
			cin >> a >> b;
			--a; --b;
			large[a][b] = large[b][a] = true;
		}
		cin >> m;
		REP(i,m-1)
		{
			int a, b;
			cin >> a >> b;
			--a; --b;
			small[a][b] = small[b][a] = true;
		}
		vector<int> order(n);
		REP(i,n) order[i] = i;
		bool copied = false;
		do
		{
			bool ok = true;
			REP(i,m) if(ok) REP(j,i)
			{
				if(small[i][j] != large[order[i]][order[j]])
				{
					ok = false; break; 
				}
			}
			if(ok) { copied = true; break; }
		} while(next_permutation(all(order)));
		if(copied)
			printf("YES\n");
		else
			printf("NO\n");
	}
}
