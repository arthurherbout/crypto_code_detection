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
#include<list>
#include<math.h>
#define cycle(i,m,n) for(int i = (m); i < (n); i++)
#define cycled(i,m,n) for(int i=(m); i > (n); i--)
#define ll long long
#define pii pair<int,int>
#define pll pair<ll,ll>
#define mp make_pair
#define endl '\n'

using namespace std;
const ll MAX = (ll)1000 * 1000 * 1000;
ifstream in("B-large.in");
ofstream out("out.txt");
void writeres(int c, vector<vector<int>>& a)
{
	out << "Case #" + to_string(c) + ": POSSIBLE" << endl;
	cycle(i, 0, a.size())
	{
		cycle(j, 0, a.size())
			out << a[i][j];
		out << endl;
	}
}
void writeresimp(int c)
{
	out << "Case #" + to_string(c) + ": IMPOSSIBLE" << endl;
}
int main()
{
	int t, b;
	unsigned ll m;
	in >> t;
	cycle(i, 0, t)
	{
		in >> b >> m;
		vector<vector<int>> res(b, vector<int>(b, 0));
		unsigned ll temp = 1;
		cycle(j, 1, b)
			cycle(k, j + 1, b)
			res[j][k] = 1;
		bool p = 1;
		cycle(j, 0, b - 2)
		{
			p = 0;
			temp <<= 1;
			if (temp >= m)
			{
				if (j == b - 3)
					p = 1;
				break;
			}
		}
		if (temp == m && p)
		{
			cycle(j, 1, b)
				res[0][j] = 1;
			writeres(i + 1, res);
			continue;
		}

		if (temp < m)
		{
			writeresimp(i + 1);
			continue;
		}

		cycle(j, 1, b - 1)
		{
			if (m & (1 << (b - j - 2)))
				res[0][j] = 1;
		}
		writeres(i + 1, res);
	}
	return 0;
}

