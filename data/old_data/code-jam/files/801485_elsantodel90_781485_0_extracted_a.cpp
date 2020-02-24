#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <deque>
#include <cstdio>
#include <cstdlib>
#include <numeric>
#include <functional>
#include <algorithm>

using namespace std;

#define forn(i,n) for(int i=0;i<(int)(n);i++)
#define forsn(i,s,n) for(int i=(s);i<(int)(n);i++)
#define dforn(i,n) for(int i=(n)-1;i>=0;i--)
#define dforsn(i,s,n) for(int i=(n)-1;i>=(int)(s);i--)

#define forall(i,c) for(typeof((c).begin()) i = (c).begin(); i != (c).end(); i++)
#define dforall(i,c) for(typeof((c).rbegin()) i = (c).rbegin(); i != (c).rend(); i++)
#define all(c) (c).begin(),(c).end()

#define esta(x,c) ((c).find(x) != (c).end())
#define zMem(c) memset((c),0,sizeof(c))

typedef long long tint;
typedef long double tdbl;

typedef pair<int,int> pint;
typedef pair<tint,tint> ptint;

typedef vector<int> vint;
typedef vector<vint> vvint;
typedef vector<string> vstr;

#define INF 1000000000

int main()
{
	freopen("entrada.in","r",stdin);
	freopen("salida.out","w",stdout);
	int totalCasos;
	cin >> totalCasos;
	forn(caso,totalCasos)
	{
		string s;
		cin >> s;
		int L0 = s.size();
		string::iterator it = unique(all(s));
		s.resize(it-s.begin());
		int L = s.size();
		vvint dp(L+15,vint(L+15));
		forn(i,L+1) dp[i][i] = 0;
		forn(i,L) dp[i][i+1] = 1;
		forsn(k,2,L+1)
		forn(i,L+1-k)
		{
			int &res = dp[i][i+k] = INF;//;1 + ((s[i]==s[i+k-1])?(dp[i+1][i+k-1]):(min(dp[i+1][i+k],dp[i][i+k-1])));
			forsn(l,i,i+k)
			{
				int acu = dp[i][l];
				int ult = l;
				forsn(r,l,i+k)
				if (s[r] == s[l])
				{
					acu += dp[ult][r];
					ult = r+1;
					int x = dp[r+1][i+k] + acu;
					if (res > x) res = x;
				}
			}
			res++;
		}
		int res = 2*dp[0][L] + L0;
		cout << "Case #" << caso + 1 << ": " << res << endl;
		cerr << "Case #" << caso + 1 << ": " << res << endl;
	}
	return 0;
}
