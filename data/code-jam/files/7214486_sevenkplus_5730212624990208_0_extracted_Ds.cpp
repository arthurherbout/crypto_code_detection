#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <bitset>
#include <numeric>
#include <ctime>
#include <cmath>
#include <cassert>
#include <algorithm>

using namespace std;

typedef pair<int, int> PII;
typedef long long ll;

#define fi first
#define se second
#define mp make_pair
#define pb push_back
#define pct __builtin_popcount

int main()
{
	int _; cin >> _;
	for (int __ = 1; __ <= _; __ ++) {
		int n,k;
		vector<string> A;
		cin >> n >> k;
		for (int i = 0; i < n; i ++) {
			string s;
			cin >> s;
			A.pb(s);
		}
		vector<int> p;
		for (int i = 0; i < n; i ++) p.pb(i);
		bool F = 0;
		do {
			int s = p[0];
			for (int i = 1; i < n; i ++) {
				int t = p[i];
				if (A[s][t] == 'N') s = t;
			}
			if (s == k) {
				F = 1;
				break;
			}
		}
		while (next_permutation(p.begin(),p.end()));
		printf ("Case #%d: ",__);
		if (!F) puts("IMPOSSIBLE"); else {
			for (int i = 0; i < n; i ++)
				printf ("%d%c",p[i],i==n-1?'\n':' ');
		}
	}
	return 0;
}
