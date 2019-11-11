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

typedef vector<int>::iterator vit;

#define N 10100
int n;
vector<int> A[N];
string c[N];

map<PII,string> E;

string ff(int x, int y) {
	PII z = mp(x,y);
	if (E.find(z) != E.end()) return E[z];
	vector<string> a;
	for (vit i = A[x].begin(); i != A[x].end(); i ++) 
		if (*i != y) a.pb(ff(*i,x));
	sort(a.begin(), a.end());
	string s;
	s += c[x];
	s += '(';
	for (int i = 0; i < (int) a.size(); i ++) 
		s += a[i];
	s += ')';
	//cout << s << endl;
	return E[z] = s;
}

bool v[N];

bool hh() {
	for (int i = 0; i < n; i ++) if (v[i]) {
		vector<string> a;
		for (vit j = A[i].begin(); j != A[i].end(); j ++) 
			if (!v[*j]) a.pb(E[mp(*j,i)]);
		if ((int)a.size()%2 != 0) return 0;
		sort(a.begin(),a.end());
		for (int j = 0; j < (int) a.size(); j += 2) 
			if (a[j] != a[j+1]) return 0;
	}
	return 1;
}

bool gg(int x) {
	v[x] = 1;
	if (hh()) return 1;
	for (vit i = A[x].begin(); i != A[x].end(); i ++) 
		if (!v[*i]) {
			if (gg(*i)) return 1;
		}
	v[x] = 0;
	return 0;
}

int main()
{
	int _; cin >> _;
	for (int __ = 1; __ <= _; __ ++) {
		cin >> n;
		for (int i = 0; i < n; i ++) A[i].clear();
		E.clear();
		for (int i = 0; i < n; i ++) cin >> c[i];
		for (int i = 1; i < n; i ++) {
			int x, y;
			cin >> x >> y;
			--x, --y;
			A[x].pb(y);
			A[y].pb(x);
		}
		for (int i = 0; i < n; i ++) 
			for (vit j = A[i].begin(); j != A[i].end(); j ++) 
				ff(i,*j);
		memset(v,0,sizeof v);
		bool F = 0;
		for (int i = 0; i < n; i ++) 
			if (gg(i)) {
				F = 1; break;
			}
		for (int i = 0; i < n; i ++) 
			for (vit j = A[i].begin(); j != A[i].end(); j ++) 
				if (E[mp(i,*j)] == E[mp(*j,i)]) F = 1;
		printf("Case #%d: %s\n",__, F?"SYMMETRIC":"NOT SYMMETRIC");
	}
	return 0;
}
