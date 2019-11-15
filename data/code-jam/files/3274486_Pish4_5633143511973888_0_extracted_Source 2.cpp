#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<vector>
#include<string>
#include<iostream>
#include<algorithm>
#include<map>
#include<iterator>
#include<set>
#include<stack>
#include<queue>
#include<fstream>
#include<iomanip>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include<cmath>
#include<list>
#include <sstream>
#define rep(i,m,n) for(int i = (m); i < (n); i++)
#define rep0(i, n) for(int i = (0); i < (n); i++)
#define repd(i,m,n) for(int i=(m); i > (n); i--)
#define ll long long
#define ull unsigned ll
#define pii pair<int,int>
#define pll pair<ll,ll>
#define plli pair<pll,int>
#define mp make_pair
#define endl '\n'
#define pb push_back
#define x first
#define y second
#define pt pair<ll,ll>
#define vi vector<int>
#define vll vector<ll>
#define vvi vector<vi>
#define vvll vector<vll>
#define vpii vector<pii>
#define vpll vector<pll>
#define vpt vector<pt>
#define vb vector<bool>
#define vvb vector<vb>
#define pdd pair<long double, long double>
#define vpll vector<pll>
#define vpdd vector<pair<long double, long double> >
#define pq priority_queue
#define M_PI 3.14159265358979323846
using namespace std;

int bitsAmount(int n) {
	int res = 0;
	rep(i, 0, 15) {
		if (n&(1 << i)) {
			res++;
		}
	}
	return res;
}
bool comp(pdd p1, pdd p2) {
	if (p1.second == p2.second) {
		return p1.first < p2.first;
	}
	return p1.second < p2.second;
}
int main() {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
	int t;
	cin >> t;
	rep(qwe, 1, t + 1) {
		cout << "Case #" << qwe << ": ";
		int n, k;
		cin >> n >> k;
		double u;
		cin >> u;
		vector<double> arr(n);
		rep(i, 0, n) {
			cin >> arr[i];
			arr[i] *= 1e5;
		}
		double lo = 0, hi = 1e5;
		double res = 0;
		while (hi - lo > 1e-6) {
			double mid = (hi + lo) / 2;
			double amount = u*1e5;
			rep(i, 0, n) {
				if (arr[i] < mid) {
					amount -= mid - arr[i];
				}
			}
			if (amount < 0) {
				hi = mid;
			}
			else {
				res = mid;
				lo = mid;
			}
		}
		double ans = 1;
		res /= 1e5;
		rep(i, 0, n) {
			ans *= max(res, arr[i]/1e5);
		}
		cout << setprecision(7) << fixed << ans << endl;
	}
	return 0;
}