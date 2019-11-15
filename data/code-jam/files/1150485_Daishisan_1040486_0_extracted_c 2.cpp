#include <algorithm>
#include <cassert>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <utility>
#include <vector>

using namespace std;

typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef complex<ld> pt;

template<class T>
T gcd(T a, T b) {
	T t;
	while (b) {
		t = b;
		b = a%b;
		a = t;
	}
	return a;
}

int main() {
	int num_tests; cin >> num_tests;
	for (int test = 1; test <= num_tests; ++test) {
		// get input
		int n, m; cin >> n >> m;
		int u[m], v[m];
		for (int i = 0; i < m; ++i)
			cin >> u[i];
		for (int i = 0; i < m; ++i)
			cin >> v[i];
		// get the regions
		set<set<int> > regions;
		set<int> init;
		for (int i = 1; i <= n; ++i)
			init.insert(i);
		regions.insert(init);
		/// go through each interior wall to get the region
		for (int i = 0; i < m; ++i) {
			for (set<set<int> >::iterator it = regions.begin(); it != regions.end(); ++it) {
				// find the region these two points are in
				if (it->find(u[i]) != it->end() && it->find(v[i]) != it->end()) {
					// split region into two
					set<int> old = *it;
					set<int> new1, new2;
					int a = min(u[i], v[i]);
					int b = max(u[i], v[i]);
					for (set<int>::iterator jt = old.begin(); jt != old.end(); ++jt) {
						if (a <= *jt && *jt <= b)
							new1.insert(*jt);
						if (b <= *jt || *jt <= a)
							new2.insert(*jt);
					}
					regions.erase(*it);
					regions.insert(new1);
					regions.insert(new2);
					break;
				}
			}
		}
		// for each vertex, find out what regions it's in
		int smallest_region_size = n;
		vector<set<int> > regions_v(regions.begin(), regions.end());
		map<int, set<int> > mp;
		for (int i = 0; i < (int) regions_v.size(); ++i) {
			smallest_region_size = min(smallest_region_size, (int) regions_v[i].size());
			for (set<int>::iterator it = regions_v[i].begin(); it != regions_v[i].end(); ++it) {
				mp[*it].insert(i);
//cout << "vertex " << *it << " is in region " << i << endl;
			}
		}
//cout << "smallest_region = " << smallest_region_size << endl;
//cout << "number of regions = " << regions.size() << endl;
		// brute force this (this isn't going to work for the large test case =P)
		int ans = -1;
		for (ans = smallest_region_size; ans >= 0; --ans) {
			bool found = true;
			for (ll i = 0; i < (ll) (pow(ld(ans), ld(n)) + 0.5); ++i) {
				found = true;
				for (int color = 0; color < ans; ++color) {
					ll ii = i;
					vector<int> vertices_that_are_color;
					for (int x = 1; x <= n; ++x) {
						int curr = ii%ans;
						if (curr == color)
							vertices_that_are_color.push_back(x);
						ii/=ans;
					}
					assert(ii == 0);
					set<int> regions_that_have_color;
					for (int x = 0; x < (int) vertices_that_are_color.size(); ++x) {
						regions_that_have_color.insert(mp[vertices_that_are_color[x]].begin(), mp[vertices_that_are_color[x]].end());
					}
					if (regions_that_have_color.size() != regions.size()) {
						found = false;
						break;
					}
				}
				if (found) {
//cout << "HI" << endl;
					cout << "Case #" << test << ": " << ans << endl;
					ll ii = i;
					for (int x = 1; x < n; ++x) {
						cout << (ii%ans) + 1 << " ";
						ii /= ans;
					}
					cout << ii + 1 << endl;
					break;
				}
			}
			if (found) {
				break;
			}
		}
	}
}

