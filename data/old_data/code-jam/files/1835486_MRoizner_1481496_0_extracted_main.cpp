#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include <bitset>
#include <set>
#include <sstream>
#include <stdlib.h>
#include <map>
#include <queue>
#include <stack>
#include <assert.h>

using namespace std;

#define sz(x) ((int)x.size())
#define all(x) (x).begin(), (x).end()
#define pb(x) push_back(x)
#define mp(x, y) make_pair(x, y)

#define forit(X,Y) for(typeof((Y).begin()) X = (Y).begin(); X != (Y).end(); ++X)

#define debug(x) cout << '>' << #x << ':' << x << endl;

typedef long long int64;

typedef vector <int> vi;
typedef vector < vi > vvi;


const double INF = 1000000;
const double EPS = 1E-6;

char transform(char c) {
	//"o" to "0", "i" to "1", "e" to "3", "a" to "4", "s" to "5", "t" to "7", "b" to "8" and/or "g" to "9".
	switch (c) {
	case 'o': return '0';
	case 'i': return '1';
	case 'e': return '3';
	case 'a': return '4';
	case 's': return '5';
	case 't': return '7';
	case 'b': return '8';
	case 'g': return '9';
	default: return c;
	}
}

int toindex(char c) {
	return '0' <= c && c <= '9' ? c - '0' : c - 'a' + 10;
}

pair<int, int> dfs(int n, vector<vector<bool> >& table, int i, vector<bool>& visited)
{
	visited[i] = true;
	pair<int64, int64> res = mp(0LL, 0LL);
	int plus = 0;
	int minus = 0;
	for (int j = 0; j < n; ++j) {
		if (table[i][j]) {
			res.first += 1;
			if (j != i) plus++;
			if (!visited[j]) {
				pair<int64, int64> r = dfs(n, table, j, visited);
				res.first += r.first;
				res.second += r.second;
			}
		}
		if (j != i && table[j][i]) {
			minus++;
			if (!visited[j]) {
				pair<int64, int64> r = dfs(n, table, j, visited);
				res.first += r.first;
				res.second += r.second;
			}
		}
	}
	if (plus > minus)
		res.second += plus - minus;
	return res;
}
int main() {
    freopen("input.txt", "rt", stdin);
    freopen("output.txt", "wt", stdout);

	int testCount;
    cin >> testCount;
    for (int testNumber = 1; testNumber <= testCount; ++testNumber) {
    	int k;
    	cin >> k;
    	assert(k == 2);
    	string s;
    	cin >> s;

    	vector<vector<bool> > table(36, vector<bool>(36));
    	for (int i = 1; i < s.length(); ++i)
    	{
    		char a = s[i - 1];
    		char b = s[i];
    		table[toindex(a)][toindex(b)] = true;
    		table[toindex(a)][toindex(transform(b))] = true;
    		table[toindex(transform(a))][toindex(b)] = true;
    		table[toindex(transform(a))][toindex(transform(b))] = true;
    	}

    	int64 sum = 0;
    	vector<bool> visited(36);
    	for (int i = 0; i < 36; ++i) {
    		if (!visited[i]) {
    			pair<int64, int64> r = dfs(36, table, i, visited);
    			if (r.first > 0) {
    				sum += r.first + max(r.second, 1LL);
    			}
    		}
    	}

    	cout << "Case #" << testNumber << ": " << sum << endl;
    }

    return 0;
}
