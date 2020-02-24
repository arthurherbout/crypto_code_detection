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
	rep(qwe, 1, t + 1) {
		int n;
		vi a(6);
		cin >> n;
		rep(i, 0, 6) {
			cin >> a[i];
		}
		char b[6] = { 'R', 'O', 'Y', 'G', 'B', 'V' };
		char went[2000];
		went[0] = 'Q';
		rep(c, 1, n + 1) {
			went[c] = 'M';
		}
		int prev;
		char prev2;
		rep(i,0,6){
			rep(k, 0, 5) {
				if (a[k] >= a[k + 1]) {
					prev = a[k];
					prev2 = b[k];
					a[k] = a[k + 1];
					b[k] = b[k + 1];
					a[k + 1] = prev;
					b[k + 1] = prev2;
				}
			}
		}
		went[1] = b[5];
		a[5]--;
		rep(c,2,n+1) {
			rep(j,0,6) {
				rep(k, 0, 5){
					if (a[k] >= a[k + 1]) {
						prev = a[k];
						prev2 = b[k];
						a[k] = a[k + 1];
						b[k] = b[k + 1];
						a[k + 1] = prev;
						b[k + 1] = prev2;
					}
				}
			}
			for (int x = 5; x >= 0; x--) {
				if (went[c - 1] == 'R' && b[x] != 'R' && b[x] != 'V' && b[x] != 'O'){
					a[x]--;
					went[c] = b[x];
					break;
				}
				else if (went[c - 1] == 'O' && b[x] != 'O' && b[x] != 'R' && b[x] != 'Y'){
					a[x]--;
					went[c] = b[x];
					break;
				}
				else if (went[c - 1] == 'Y' && b[x] != 'Y' && b[x] != 'G' && b[x] != 'O'){
					a[x]--;
					went[c] = b[x];
					break;
				}
				else if (went[c - 1] == 'G' && b[x] != 'G' && b[x] != 'Y' && b[x] != 'V'){
					a[x]--;
					went[c] = b[x];
					break;
				}
				else if (went[c - 1] == 'B' && b[x] != 'B' && b[x] != 'V' && b[x] != 'G'){
					a[x]--;
					went[c] = b[x];
					break;
				}
				else if (went[c - 1] == 'V' && b[x] != 'V' && b[x] != 'R' && b[x] != 'B'){
					a[x]--;
					went[c] = b[x];
					break;
				}
			}
		}
		int temp = 0;
		rep(j, 1, n + 1) {
			if (went[j] == 'M') {
				temp = 1;
			}
		}
		if (went[n] == went[1]) {
			temp = 1;
		}
		cout << "Case #" << qwe << ": ";
		if (temp) {
			cout << "IMPOSSIBLE" << endl;
		}
		else
		{
			rep(j, 1, n + 1) {
				cout << went[j];
			}
			cout << endl;
		}
	}
	return 0;
}