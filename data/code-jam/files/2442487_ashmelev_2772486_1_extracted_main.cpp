#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <bitset>
#include <queue>
#include <stack>
#include <sstream>
#include <cstring>
#include <numeric>
#include <ctime>

#define re return
#define fi first
#define se second
#define mp make_pair
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define sz(x) ((int) (x).size())
#define rep(i, n) for (int i = 0; i < (n); i++)
#define rrep(i, n) for (int i = (n) - 1; i >= 0; i--)
#define y0 y32479
#define y1 y95874
#define fill(x, y) memset(x, y, sizeof(x))
#define sqr(x) ((x) * (x))
#define sqrt(x) sqrt(abs(x))
#define unq(x) (x.resize(unique(all(x)) - x.begin()))
#define spc(i,n) " \n"[i == n - 1]

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<string> vs;
typedef double D;
typedef long double LD;
typedef long long ll;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;

template<class T> T abs(T x) { return x > 0 ? x : -x;}

int m;
int n;

ll s1;
ll s2;

ll mod = 1000002013;

ll get2(ll x) {
	re ((x * (x + 1)) / 2) % mod;
}

ll get(ll x) {
	re (get2(n) - get2(n - x) + mod) % mod;
}

vi v;
map<int, ll> c1, c2;

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tc;
	cin >> tc;
	rep(tt, tc) {
		v.clear();
		c1.clear();
		c2.clear();

		cout << "Case #" << tt + 1 << ": ";
		cin >> n >> m;
		s1 = 0;
		rep(i, m) {
			ll a, b, c;
			cin >> a >> b >> c;
			a--;
			b--;
			c1[a] += c;
			c2[b] += c;
			s1 += get(b - a) * c;
			s1 %= mod;
			v.pb(a);
			v.pb(b);
		}

		sort(all(v));
		unq(v);

		s2 = 0;
		rep(i, sz(v)) {
			ll cur = c2[v[i]];
			for (int j = i; j >= 0; j--) {
				ll o = min(cur, c1[v[j]]);
				if (o > 0){
					s2 += get(v[i] - v[j]) * o;
					s2 %= mod;
					cur -= o;
					c1[v[j]] -= o;
				}
			}
		}

		cout << (s1 - s2 + mod) % mod << endl;
	}



	re 0;
}

