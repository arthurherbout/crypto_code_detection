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

int main()
{
	int cases;
	cin >> cases;
	REP(cc,cases)
	{
		fprintf(stderr, "%d of %d ..\r", cc+1, cases);
		printf("Case #%d:", cc+1);
		int n;
		cin >> n;
		int h, w;
		string a, b;
		vector<int> birdHeight, birdWeight;
		vector<int> notHeight, notWeight;
		vector<int> weights, heights;
		REP(i,n)
		{
			cin >> h >> w >> a;
			if(a == "NOT") cin >> b;
			if(a == "NOT")
			{
				notHeight.pb(h);
				notWeight.pb(w);
			}
			else
			{
				birdHeight.pb(h);
				birdWeight.pb(w);
			}
			weights.pb(w);
			heights.pb(w);
		}
		sort(all(birdHeight));
		sort(all(birdWeight));
		vector<int> suspectHeight, suspectWeight;
		printf("\n");
		int m;
		cin >> m;
		REP(i,m)
		{
			cin >> h >> w;
			suspectHeight.pb(h); suspectWeight.pb(w);
			weights.pb(w);
			heights.pb(h);
		}
		sort(all(weights));
		sort(all(heights));
		weights.erase(unique(all(weights)), weights.end());
		heights.erase(unique(all(heights)), heights.end());
		if(birdHeight.sz == 0)
		{
			REP(i,m) printf("UNKNOWN\n");
		}
		else
		{
			vector<bool> canBird(m), canNoBird(m);
			REP(h1,heights.sz) if(heights[h1] <= birdHeight[0])
				FOR(h2,h1,heights.sz) if(birdHeight.back() <= heights[h2])
				REP(w1,weights.sz) if(weights[w1] <= birdWeight[0])
				FOR(w2,w1,weights.sz) if(birdWeight.back() <= weights[w2])
				{
					bool ok = true;
					REP(i,notHeight.sz)
					{
						if(heights[h1] <= notHeight[i] && notHeight[i] <= heights[h2] &&
								weights[w1] <= notWeight[i] && notWeight[i] <= weights[w2])
						{
							ok = false;
							break;
						}
					}
					if(!ok) continue;
					REP(i,m)
					{
						if(
								heights[h1] <= suspectHeight[i] && suspectHeight[i] <= heights[h2] &&
								weights[w1] <= suspectWeight[i] && suspectWeight[i] <= weights[w2]
						  )
						{
							canBird[i] = true;
						}
						else
						{
							canNoBird[i] = true;
						}							
					}
				}
			REP(i,m)
			{
				if(canBird[i] && canNoBird[i])
					printf("UNKNOWN\n");
				else if(canBird[i])
					printf("BIRD\n");
				else if(canNoBird[i])
					printf("NOT BIRD\n");
				else
					printf("UNKNOWN\n");

			}
		}
	}
}
