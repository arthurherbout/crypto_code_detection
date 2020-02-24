#include <cassert>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <set>
#include <map>
#include <stack>
#include <list>
#include <queue>
#include <deque>
#include <cctype>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <numeric>
#include <cmath>
using namespace std;

typedef vector <int > VI;
typedef vector < VI > VVI;
typedef long long LL;
typedef vector < LL > VLL;
typedef vector < double > VD;
typedef vector < string > VS;
typedef pair<int,int> PII;
typedef vector <PII> VPII;
typedef istringstream ISS;

#define ALL(x) (x).begin(),(x).end()
#define REP(i,n) for (int i=0; i<(n); ++i)
#define FOR(var,pocz,koniec) for (int var=(pocz); var<=(koniec); ++var)
#define FORD(var,pocz,koniec) for (int var=(pocz); var>=(koniec); --var)
#define FOREACH(it, X) for(__typeof((X).begin()) it = (X).begin(); it != (X).end(); ++it)
#define PB push_back
#define PF push_front
#define MP(a,b) make_pair(a,b)
#define ST first
#define ND second
#define SIZE(x) (int)(x).size()

inline int between(LL a, LL b, LL c){
  return a >= b && a <= c;
}

const LL INF = 1000LL * 1000 * 1000 * 1000 * 1000 * 1001;
const int K = 22;
const int S = 17;

set<LL> t[K+5][1<<S][S+1];

inline LL ABS(LL x){
	return x >= 0 ? x : -x;
}

vector<set<LL> > licz(vector<LL> pozycje, LL granica){
	assert(SIZE(pozycje)+1 <= S);
	int n = SIZE(pozycje);
	pozycje.PB(granica);
	int ile = 1<<n;
	FOR(ile_razy,0,K) REP(mask,ile) FOR(last,0,n) t[ile_razy][mask][last].clear();

	int ktory = -1;
	REP(i,n) if (pozycje[i] == 0){
		assert(ktory == -1);
		ktory = i;
	}
	if (ktory >= 0){
		t[0][0][ktory].insert(0);
	} else {
		t[0][0][n].insert(0);
	}

	FOR(ile_razy,0,K) REP(mask,ile) FOR(last,0,n){
		const set<LL> &kosztset = t[ile_razy][mask][last];
		FOREACH(it,kosztset){
			LL koszt = *it;
			FOR(next, 0, n) if (next != last){
				if (next == n || ((1<<next) & mask) == 0){
					t[ile_razy + (next == n)][mask + ((next == n) ? 0 : (1<<next))][next].insert(koszt+ABS(pozycje[last]-pozycje[next]));
				}
			}
		}
	}
	vector<set<LL > > res;
	FOR(ile_razy,1,K){
		int last = (ktory >= 0 ? ktory : n);
		res.PB(t[ile_razy][ile-1][last]);
	}
	return res;
}

int main(int argc, char **argv){
  int testy;
  scanf("%d",&testy);
  int numer=0;
  while (testy--){
    int n;
	scanf("%d",&n);
	vector<LL> pozycje;
	REP(i,n){
		LL x;
		scanf("%lld",&x);
		x *= 2;
		pozycje.PB(x);
	}
	LL paliwo;
	scanf("%lld",&paliwo);
	paliwo *= 2;
	LL res = INF;

	sort(ALL(pozycje));

	vector<LL> lewe, prawe;
	REP(i, n) if (i < n/2) lewe.PB(pozycje[i]);
	else prawe.PB(pozycje[i]);
	assert(!lewe.empty());
	assert(!prawe.empty());
	LL granica = (lewe.back() + prawe[0]) / 2;

	vector<set<LL> > lewy_res = licz(lewe, granica);
	vector<set<LL> > prawy_res = licz(prawe, granica);

	assert(SIZE(lewy_res) == K && SIZE(prawy_res) == K && 2 * K > n+3);

	REP(i,K){
		FOREACH(it, lewy_res[i]) if (*it <= paliwo){
			set<LL>::iterator it2 = prawy_res[i].upper_bound(paliwo-*it);
			if (it2 != prawy_res[i].begin()){
				--it2;
				LL kand = *it + *it2;
				if (kand <= paliwo){
					if (res == INF || res < kand) res = kand;
				}
			}
		}
	}

    ++numer;
	if (argc == 3 && numer % atoi(argv[1]) != atoi(argv[2])){
		continue;
	}
    printf("Case #%d: ",numer);
	if (res == INF) puts("NO SOLUTION");
	else{
		assert(res % 2 == 0);
	   	printf("%lld\n", res / 2);
	}
    fflush(stdout);
  }
  return 0;
}
