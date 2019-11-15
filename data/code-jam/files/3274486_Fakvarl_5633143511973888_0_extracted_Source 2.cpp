#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <set>
#include <map>
#include <list>
#include <functional>
#include <math.h>
#include <iomanip>
#include <fstream>
using namespace std;

#define rep0(i, n) for(int i = 0; i < (n); ++i)
#define ll long long
#define pll pair<ll, ll>
#define vll vector<ll>
#define vpll vector<pll>
#define vvll vector<vll>

#define vb vector<bool>
#define has(a, n) (a.find(n) != a.end())

int prefix_function(string s, int m) {
	int n = (int)s.length();
	vector<int> pi(n);
	for (int i = 1; i < n; ++i) {
		int j = pi[i - 1];
		while (j > 0 && s[i] != s[j])
			j = pi[j - 1];
		if (s[i] == s[j])  ++j;
		pi[i] = j;
		if (pi[i] == m) {
			return i;
		}
	}

	return -1;
}

int main() {
	ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
	freopen("Cs.in", "r", stdin);
	double  M_PI = 3.141592653589793238462643383279502884;
	freopen("Cout.txt", "w", stdout);
	int t;
	int interval = 12 * 60;
	cin >> t;
	for (int qt = 0; qt < t; ++qt) {
		int n, k;
		double units;
		cin >> n >> k >>  units;
		vector<double> probs(n);
		for (int i = 0; i < n; ++i) {
			cin >> probs[i];
		
		}
		double l = 0, r = 1, res = 0;
		while (r - l > 1e-12) {
			double mid = (l + r) / 2;
			double sm = units;
			for (int i = 0; i < n; ++i) {
				if (probs[i] < mid) {
					sm -= mid - probs[i];
				}
			}

			if (sm < 0) {
				r = mid;
			}
			else {
				res = mid;
				l = mid;
			}
		}
		double finalRes = 1;
		for (int i = 0; i < n; ++i) {
			finalRes *= max(res, probs[i] );
		}

;
		cout << "Case #" << qt + 1 << ": " << setprecision(7) << fixed << finalRes  << endl;

		
	}

}