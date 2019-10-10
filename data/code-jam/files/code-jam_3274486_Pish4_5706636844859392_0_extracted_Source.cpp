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
		int a1, a2;
		cin >> a1 >> a2;
		vpii camer(a1), jamie(a2);
		rep(i,0,a1) {
			cin >> camer[i].first >> camer[i].second;
		}
		sort(camer.begin(), camer.end());
		rep(i,0,a2) {
			cin >> jamie[i].first >> jamie[i].second;
		}
		sort(jamie.begin(), jamie.end());

		if (a1 + a2 <= 1 || (a1 == 1 && a2 == 1)){
			cout << 2 << endl;
			continue;
		}
		
		if (a1 == 2) {
			int delta1 = camer[1].second - camer[0].first;
			if (delta1 <= 0) {
				delta1 += 720 * 2;
			}
			int delta2 = camer[0].second - camer[1].first;
			if (delta2 <= 0) {
				delta2 += 720 * 2;
			}

			if (min(delta1, delta2) <= 720) {
				cout << 2 << endl;
				continue;
			}
			else {
				cout << 4 << endl;
			}
		}
		else {
			int delta1 = jamie[1].second - jamie[0].first;
			if (delta1 <= 0) {
				delta1 += 720 * 2;
			}
			int delta2 = jamie[0].second - jamie[1].first;
			if (delta2 <= 0) {
				delta2 += 720 * 2;
			}

			if (min(delta1, delta2) <= 720) {
				cout << 2 << endl;
				continue;
			}
			else {
				cout << 4 << endl;
			}
		}
	}
	return 0;
}