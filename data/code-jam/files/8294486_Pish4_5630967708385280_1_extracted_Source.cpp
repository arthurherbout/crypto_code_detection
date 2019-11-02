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
#include <random>       // std::default_random_engine
#include <chrono>
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
#define pdd pair<double, double>
#define vpll vector<pll>
#define vpdd vector<pair<double, double> >
#define pq priority_queue

using namespace std;


int main() {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);


	int t;
	cin >> t;
	rep(qwe,1,t+1) {
		long double d;
		int n;
		cin >> d >> n;
		vpll arr(n);
		rep(i, 0, n) {
			cin >> arr[i].first >> arr[i].second;
		}

		long double lo = 0, hi = 1e18;
		long double res = -1;
		rep(i,1,152) {
			long double mid = lo + (hi-lo)/2;
			bool good = true;
			rep(i, 0, n) {
				long double nearSpeed = mid - arr[i].second;
				if (nearSpeed < 0) {
					continue;
				}
				long double initDist = arr[i].first;
				long double time = initDist / nearSpeed;
				long double point = time * mid;
				if (d > point ) {
					good = false;
				}
			}
			if (good){
				res = mid;
				lo = mid*(1-1e-9);
			}
			else {
				hi = mid*(1+1e-9);
			}
		}
		cout << "Case #" << qwe << ": " << setprecision(7) << fixed << res << endl;
	}

	return 0;
}