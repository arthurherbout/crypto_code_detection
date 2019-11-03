#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <functional>
#define int long long
#define double long double
using namespace std;
typedef pair<int, int> P;

double PAI = 3.14159265358979;

int t;
int n, k;
vector<P> rh;	//rが小さい順にソートしてある

double solve() {
	int i;	//一番下のパンケーキの番号
	double ans = 0;
	
	for (i = k - 1; i < n; i++) {
		vector<int> vec;
		
		for (int j = i - 1; j >= 0; j--) {
			vec.push_back(rh[j].first * rh[j].second);
		}
		sort(vec.begin(), vec.end(), greater<int>());
		
		int maxSokumen = 0;
		for (int j = 0; j < k - 1; j++) {
			maxSokumen += vec[j];
		}
		maxSokumen += rh[i].first * rh[i].second;
		
		double score = 2 * PAI * maxSokumen + PAI * rh[i].first * rh[i].first;
		ans = max(ans, score);
	}
	return ans;
}

signed main() {
	int id, i;
	
	cin >> t;
	for (id = 1; id <= t; id++) {
		cin >> n >> k;
		
		for (i = 0; i < n; i++) {
			int r, h; cin >> r >> h;
			rh.push_back(P(r, h));
		}
		sort(rh.begin(), rh.end());
		
		double ans = solve();
		printf("Case #%lld: %.14Lf\n", id, ans);
		rh.clear();
	}
	return 0;
}