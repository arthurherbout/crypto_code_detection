#include <vector>
#include <map>
#include <set>
#include <stack>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <string>
#include <cctype>
#include <cstring>
#include <queue>
#include <cassert>

using namespace std;

typedef long long LL;
typedef long double LD;
typedef vector<int> VI;
typedef pair<int,int> PII;
typedef vector<PII> VPII;

#define MP make_pair
#define ST first
#define ND second
#define PB push_back
#define FOR(i,a,b) for( int i=(a); i<(b); ++i)
#define FORD(i,a,b) for( int i=(a); i>(b); --i)
#define REP(i,n) for(int i=0; i<(n); ++i)
#define ALL(X) (X).begin(),(X).end()
#define SZ(X) (int)(X).size()
#define FORE(it,X) for(__typeof((X).begin()) it=(X).begin(); it!=(X).end();++it)

struct ele {
	int c,s,t;

	ele() {}
	ele(int _c,int _s,int _t)
	{
		c=_c;
		s=_s;
		t=_t;
	}
};

vector<ele> dt,rem;
int n,m;
int turn;
vector<ele> card;
VI used;
int get;
int score;

void use(int i)
{
	used[i]=true;
	if (card[i].c && get<SZ(rem))
		card.PB(rem[get++]);

	score += card[i].s;
	turn += card[i].t;
}

int main()
{
	int tn;
	cin>>tn;

	while (tn--) {
		cin>>n;
		dt.clear();
		rem.clear();
		REP(i,n) {
			int c,s,t;
			cin>>c>>s>>t;
			dt.PB(ele(c,s,t));
		}
		cin>>m;
		REP(i,m) {
			int c,s,t;
			cin>>c>>s>>t;
			rem.PB(ele(c,s,t));
		}

		int dp=0;
		REP(p,m+1) {
			turn=1;
			card=dt;
			used = VI(n+m,0);
			get=0;
			score=0;
			int capa=p;
			while (turn) {
				turn--;
				int select = -1;
				REP(i,SZ(card)) if (!used[i]) {
					if (card[i].t > 0) {
						select = i;
						break;
					}
				}

				if (select>=0) {
					use(select);
					continue;
				}
				
				if (capa) {
					int sco,idx;
					sco=idx=-1;
					REP(i,SZ(card)) if (!used[i]) {
						if (card[i].c > 0 && card[i].s > sco) {
							sco=card[i].s;
							idx=i;
						}
					}

					if (idx>=0) {
						capa--;
						use(idx);
						continue;
					}
				}

				int sco=-1,idx=-1;
				REP(i,SZ(card)) if (!used[i]) {
					if (card[i].s > sco) {
						sco=card[i].s;
						idx=i;
					}
				}

				if (idx==-1) break;
				use(idx);
			}
			dp=max(dp,score);
		}
		
		static int qq=0;
		printf("Case #%d: ",++qq);
		cout<<dp<<endl;
	}
	return 0;
}
