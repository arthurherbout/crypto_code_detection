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

#define DEBUG(x) fout << '>' << #x << ':' << x << endl;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define FOR2(i, a, b) for (int i = (a); i > (b); --i)
inline bool EQ(double a, double b) { return fabs(a-b) < 1e-9; }
const int INF = 1 << 29;

inline int two(int n) { return 1 << n; }
inline int test(int n, int b) { return n & two(b); }
inline void set_bit(int & n, int b) { n |= two(b); }
inline void unset_bit(int & n, int b) { n &= ~two(b); }
inline int last_bit(int n) { return n & (-n); }
#ifdef WIN32
typedef __int64 ll;
#else
typedef long long ll;
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

ofstream fout("output.out");
ifstream fin("input.in");

const int MAX = 1000047;
int tree[MAX];
int final[MAX];

void add(int index, int val)
{
	for (; index < MAX; index += last_bit(index))
		tree[index] += val;
}

int get(int index)
{
	int res = 0;
	for (; index; index -= last_bit(index))
		res += tree[index];
	return res;
}

int main()
{
	int T;
	fin >> T;

	FOR(step, 0, T)
	{
		int K, n;
		fin >> K >> n;

		vector<int> cards(n);
		FOR(i, 0, n) fin >> cards[i];

		memset(tree, 0, sizeof(tree));
		FOR(i, 1, K+1) add(i, 1);

		int index = 1;
		FOR(i, 1, K+1)
		{
			int fr = i;
			if (get(K) - get(index-1) < fr)
			{
				fr -= get(K) - get(index-1);
				index = 1;
				fr %= get(K);
				if (!fr) fr = get(K);
			}

			int begin = index, end = K, res = K;
			while (begin <= end)
			{
				int m = (begin+end) / 2;

				int num = get(m) - get(index-1);
				if (num >= fr)
				{
					res = min(res, m);
					end = m-1;
				}
				else begin = m+1;
			}

			add(res, -1);
			final[res] = i;
			index = res;
		}

		fout << "Case #" << step+1 << ":";
		FOR(i, 0, cards.size())
			fout << " " << final[cards[i]];
		fout << endl;
	}

	return 0;
}
