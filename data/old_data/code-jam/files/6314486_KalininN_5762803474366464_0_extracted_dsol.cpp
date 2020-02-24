#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ld = long double;
using D = double;
using uint = unsigned int;
template<typename T>
using pair2 = pair<T, T>;

#ifdef WIN32
    #define LLD "%I64d"
#else
    #define LLD "%lld"
#endif

#define pb push_back
#define mp make_pair
#define all(x) (x).begin(),(x).end()
#define fi first
#define se second

inline int sgn(int x) { return x > 0 ? 1 : (x < 0 ? -1 : 0);}

const int maxn = 55;

int x[maxn], y[maxn], z[maxn];
int n;

bool same(int a, int b)
{
	return x[a] == x[b] && y[a] == y[b] && z[a] == z[b];
}


int main()
{
    int NT = 0;
    scanf("%d", &NT);
    for (int T = 1; T <= NT; T++)
    {
        printf("Case #%d: ", T);
        
		scanf("%d", &n);
		for (int i = 0; i < n; i++)
		{
			scanf("%d%d%d", &x[i], &y[i], &z[i]);
			int g = abs(__gcd(x[i], __gcd(y[i], z[i])));
			x[i] /= g;
			y[i] /= g;
			z[i] /= g;
		}
		bool answer = false;
		for (int i = 0; i < n; i++)
		{
			for (int j = i + 1; j < n; j++)
			{
				vector<int> zeros;
				bool waspos = false;
				bool wasneg = false;
				ll xx = (ll)y[i] * z[j] - (ll)y[j] * z[i];
				ll yy = (ll)z[i] * x[j] - (ll)z[j] * x[i];
				ll zz = (ll)x[i] * y[j] - (ll)x[j] * y[i];
				if (xx == 0 && yy == 0 && zz == 0) continue;
				for (int k = 0; k < n; k++)
				{
					ll cur = xx * x[k] + yy * y[k] + zz * z[k];
					if (cur == 0) zeros.pb(k);
					else if (cur > 0) waspos = true;
					else if (cur < 0) wasneg = true;
				}
				if (waspos && wasneg) continue;
				bool ok = true;
				for (auto t : zeros)
				{
					ll xxx = (ll)y[i] * z[t] - (ll)y[t] * z[i];
					ll yyy = (ll)z[i] * x[t] - (ll)z[t] * x[i];
					ll zzz = (ll)x[i] * y[t] - (ll)x[t] * y[i];
					if (xxx == 0 && yyy == 0 && zzz == 0 && same(i, t)) continue;
					ok &= sgn(xxx) == sgn(xx) && sgn(yyy) == sgn(yy) && sgn(zzz) == sgn(zz);
				}
				if (ok) answer = true;
			}
		}
		if (answer) printf("NO\n");
		else printf("YES\n");

		
		
        fprintf(stderr, "%d/%d cases done!\n", T, NT);
    }
    return 0;
}
