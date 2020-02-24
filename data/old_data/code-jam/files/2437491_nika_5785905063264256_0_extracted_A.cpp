#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
#include <sstream>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
#define SZ(x) (int)(x.size())
#define F0(i,n) for(i=0;i<n;i++)
#define F1(i,n) for(i=1;i<=n;i++)
#define CL(a,x) memset(x, a, sizeof(x))
const int inf = 1000000009;
const double pi = atan(1.0)*4.0;
const double eps = 1e-8;
ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }
int bc(int n) { return n ? bc((n-1)&n)+1 : 0; }

int i, j, k, m, n, l, ans, x, L;
int s[1005], e[1005], t[1005], ok1[1005][1005], ok2[1005][1005], ok3[1005][1005];
char ss[1000002];

int ne(int x)
{
	return x % L + 1;
}
int pr(int x)
{
	return (x + L - 2) % L + 1;
}

int main() {
	//freopen("a.in", "r", stdin);

	freopen("A-small-attempt7.in", "r", stdin);
	freopen("A-small-attempt7.out", "w", stdout);

//	freopen("A-large.in", "r", stdin);
//	freopen("A-large.out", "w", stdout);

	int tt, tn; cin >> tn;
	F1(tt,tn) {
        cerr << tt << endl;
		printf("Case #%d: ", tt);

		cin >> n >> x >> L;
		CL(0, ok1);
		CL(0, ok2);
		CL(0, ok3);
		for (int i = 0; i < n; i++)
		{
			cin >> s[i] >> e[i] >> t[i];
			int at = t[i];
			ok3[s[i]][at] = 1;
			for (j = s[i]; j != e[i]; j = ne(j))
			{
				ok1[j][at] = 1;
				at++;
			}
			ok2[j][at] = 1;
		}
		
		ans = 0;
		for (int i = 0; i <= x; i++)
			for (int k = i + 1; k <= x; k++) if (k - i > ans)
				for (j = 1; j <= L; j++)
				{
					bool sg = 1;

					int at = j;
					if (ok2[at][i]) sg = 0;
					if (ok3[at][i]) sg = 0;
					for (int t = i; t < k; t++)
					{
					//	if (ok1[at][t]) sg = 0;
						if (ok2[at][t]) sg = 0;
						if (ok1[pr(at)][t]) sg = 0;
						if (ok1[pr(pr(at))][t]) sg = 0;
						at = pr(at);
					}
					//if (ok2[at][i]) sg = 0;
					if (ok3[at][i]) sg = 0;

					if (sg)
					{
						ans = k - i;
					}
				}
		cout << ans << endl;
	}
    cerr << "Press any key to continue" << endl;
    while (1);
	
	return 0;
}
