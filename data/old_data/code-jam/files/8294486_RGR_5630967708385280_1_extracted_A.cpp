#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using vl = vector<ll>;
using vvl=vector<vl>;
using vb=vector<bool>;
using vs=vector<string>;
using pll=pair<ll,ll>;
const ll oo = 0x3f3f3f3f3f3f3f3fLL;
const double eps = 1e-9;
#define sz(c) ll((c).size())
#define all(c) begin(c),end(c)
#define mp make_pair
#define mt make_tuple
#define pb push_back
#define eb emplace_back
#define xx first
#define yy second
#define FR(i,a,b) for(ll i = (a); i < (b); i++)
#define FRD(i,a,b) for(ll i = ll(b)-1;i>=(a);i--)
#define TR(X) ({if(1) cerr << "TR: " << (#X) << " = " << (X) << endl; })

int main() {
	ios_base::sync_with_stdio(false);
	ll tc;
	cin >> tc;
	FR(a,0,tc) {
		double dest;
		ll num;
		cin >> dest >> num;
		double slowest = .0;
		FR(b,0,num) {
			double pos, speed;
			cin >> pos >> speed;
			double goalr = (dest - pos) / speed;
			slowest = max(slowest, goalr);
		}
		double sped = dest / slowest;
		cout.precision(12);
		cout << "Case #" << (a+1) << ": " << fixed << sped << '\n';
	}
}
