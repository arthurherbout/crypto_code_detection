#include <bits/stdc++.h>

using namespace std;

#define pb push_back
#define error(x) cout << #x << " = " << x << "\n"
#define sz(a) int(a.size())

typedef long long int64;
typedef pair<int64, int64> ii;

int N, R, Y, B, O, G, V;

void solve() {
	if (O == 0 && G == 0 && V == 0) {
		vector<pair<int, char> > a;
		a.pb(make_pair(R, 'R'));	
		a.pb(make_pair(Y, 'Y'));	
		a.pb(make_pair(B, 'B'));
		sort(a.begin(), a.end());

		string res = "";
		for (int i = 0; i < a[2].first; i++)
			res += a[2].second;
		for (int i = 0, j = a[2].first-1; i < a[1].first; i++, j--) {
			res.insert(j, string(1, a[1].second));
		}
		for (int i = 0, j = 0; i < a[0].first; i++) {
			res.insert(j, string(1, a[0].second));
			j += 2;
		}
		for (int i = 0; i < sz(res); i++) {
			int j = (i + 1) % sz(res);
			if (res[i] == res[j]) {
				res = "IMPOSSIBLE";
				break;
			}
		}
		cout << res << "\n";
	}
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	ios::sync_with_stdio(0);
	int T; cin >> T;
	for (int te = 1; te <= T; te++) {
		cin >> N >> R >> O >> Y >> G >> B >> V;
		cout << "Case #" << te << ": ";
		solve();
	}

	return 0;
}