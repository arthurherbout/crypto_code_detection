#include <string>
#include <cctype>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <cstdlib>
#include <cstdio>
#include <queue>
#include <stack>
#include <memory.h>
#include <assert.h>
using namespace std;
#define SZ(a) (int)(a).size()
#define FOR(i,a,b) for (int i=(a); i<=(b); ++i)
#define REP(i,n) for (int i=0; i<(n); ++i)
#define ALL(c) c.begin(), c.end()
#define CLR(c,n) memset(c,n,sizeof(c))
#define TR(container, it) for(typeof(container.begin()) it = container.begin(); it != container.end(); it++)
#define CONTAIN(container, it) (container.find(it)!=container.end())
#define MCPY(dest,src) memcpy(dest,src,sizeof(src))
typedef long long LL;
typedef vector<int> VI;
typedef vector<string> VS;
typedef pair<int, int> PII;
const double EPS=1e-9;
const double PI=acos(-1);
const int INF=0x3F3F3F3F;
const int MX=100001;
LL l;
int n, b[100];
int cnt[MX], sum[MX];
int gcd(int a, int b) {return a?gcd(b%a,a):b;}
int main(int argc, char *argv[])
{
	freopen("B-small-attempt0.in", "r", stdin);
	freopen("B.error", "w", stdout);
	int test_case;
	scanf("%d", &test_case);
	for (int test_case_id=1; test_case_id<=test_case; ++test_case_id) {
		fprintf(stderr, "Case %d of %d\n", test_case_id, test_case);
		cin>>l>>n; REP(i,n) cin>>b[i]; sort(b,b+n);
		int g=b[0]; REP(i,n) g=gcd(g,b[i]);
		printf("Case #%d: ", test_case_id);
		if (l%g) {
			printf("IMPOSSIBLE\n");
			continue;
		} else if (l%b[n-1]==0) {
			printf("%lld\n", l/b[n-1]);
			continue;
		}
		int mx=b[n-1];
		CLR(cnt,-1); CLR(sum,-1); cnt[0]=0; sum[0]=0;
		REP(id,n-1) {
			int cur=b[id];
			REP(i,mx) if (cnt[i]!=-1) {
				for (int j=i, k=(j+cur)%mx; cnt[k]==-1||cnt[k]>cnt[j]+1; j=k, k=(j+cur)%mx) {
					cnt[k]=cnt[j]+1;
					sum[k]=sum[j]+cur;
				}
			}
		}
		int sid=l%mx;
		if (cnt[sid]==-1||sum[sid]>l) {
			printf("IMPOSSIBLE\n");
			continue;
		} else {
			printf("%lld\n", cnt[sid]+(l-sum[sid])/mx);
		}
	}
}

