#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>

using namespace std;

#define fi first
#define se second

#define Rep(i,n) for(int i=0;i<(n);++i)
#define For(i,a,b) for(int i=(a);i<=(b);++i)
#define Ford(i,a,b) for(int i=(a);i>=(b);--i)

#define MP make_pair
#define pb push_back

#define sqr(x) ((x)*(x))

const int MOD = 10007;

char a[15][15];
int m, n;
map<vector<int>, int> ma[26][15];

void viet(vector<int> v) {
	cout << "[";
	for(int i=0;i<v.size();++i) cout << v[i] << " ";	
	cout << "]" << endl;
}

int solve(char pos, int last, vector<int> v) {
	// cout << (char)(pos + 'a') << " " << last << " "; viet(v);
	{
		// check
		bool ok = true;
		for(int i=0;i<m;++i) if(v[i] != n) ok = false;
		if(ok) return 1;
	}
	
	if(pos >= 26) return 0;
	
	if(ma[pos][last].count(v)) return ma[pos][last][v];
	
	int res = 0;
	res = solve( pos + 1, 0, v);
	// add pos
	for(int i=0;i<m;++i) if(v[i] < n && (i==0 || v[i] < v[i-1])) {
		if(last <= i) {
			char c = a[i][v[i]];
			if(c == '.' || c == (pos + 'a')) {
				vector<int> u = v;
				u[i]++;
				res = (res + solve( pos, i, u)) % MOD;
			}	
		}
	}
	ma[pos][last][v] = res;
	return res;
}

int main() {
	int ntest;
	scanf("%d", &ntest);
	for(int test=1;test<=ntest;++test) {
		scanf("%d%d", &m, &n);
		gets(a[0]);
		for(int i=0;i<m;++i) gets(a[i]);
		for(int i=0;i<26;++i) for(int j=0;j<15;++j) ma[i][j].clear();
		
		vector<int> cur(m, 0);
		int res = solve(0, 0, cur);
		cout << "Case #" << test << ": " << res << endl;
	}
	return 0;
}

