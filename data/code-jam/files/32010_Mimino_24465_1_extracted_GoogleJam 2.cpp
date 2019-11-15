#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cctype>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <queue>
#include <algorithm>
using namespace std; 

#define DEBUG(x) cerr << '>' << #x << ':' << x << endl;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define FOR2(i, a, b) for (int i = (a); i > (b); --i)
inline bool EQ(double a, double b) { return fabs(a-b) < 1e-9; }
const int INF = 1 << 29;

inline int two(int n) { return 1 << n; }
inline int test(int n, int b) { return n & two(b); }
inline void set_bit(int & n, int b) { n |= two(b); }
inline void unset_bit(int & n, int b) { n &= ~two(b); }
inline int last_bit(int n) { return n & (-n); }
inline int ones(int n)
{
	int res = 0;
	while (n) { ++res; n -= last_bit(n); }
	return res;
}
#ifdef WIN32
typedef __int64 ll;
#else
typedef long long ll;
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

ifstream fin("input.in");
ofstream fout("output.out");
//FILE *fin  = fopen ("input.in", "r");
//FILE *fout = fopen ("output.out", "w");

ll MOD = 1000000009;

int N; ll K;
vector<int> tree[547];

//--pocet mojich deti
//--dedko
ll go(int parent, int node, ll colors, int num1, int num2) //num1 = dedko, num2 = pocet rodicovych surodencom
{
	ll children = tree[node].size();
	if (parent != -1) --children;

	//ofarbi si deti
	if (colors < children) return 0;
	ll res = 1;
	FOR(i, 0, children) res = (res * (ll)(colors-i)) % MOD;

	FOR(i, 0, tree[node].size())
		if (tree[node][i] != parent)
			res = (res * go(node, tree[node][i], colors-children+num1+max(0, num2-1), min(num2, 1), children)) % MOD;
	return res;
}

int main()
{
	int T;
	fin >> T;

	FOR(step, 0, T)
	{
		fin >> N >> K;
		FOR(i, 0, N) tree[i].clear();
		FOR(i, 1, N)
		{
			int A, B;
			fin >> A >> B;
			tree[A-1].push_back(B-1);
			tree[B-1].push_back(A-1);
		}
		fout << "Case #" << step+1 << ": " << go(-1, 0, K, 0, 0) << endl;
	}

	return 0;
}
