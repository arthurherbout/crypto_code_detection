#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
 
using namespace std;
 
#define RP(a,h) for(a=0; a<(h); a++)
#define FR(a,l,h) for(a=(l); a<=(h); a++)
#define GMAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define GMIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define SZ(a) (LL)a.size()
#define ALL(a) a.begin(), a.end()
#define pb push_back
typedef vector <int> VI;
typedef vector <string> VS;
typedef pair<int, int> PII;
typedef pair<PII, int> PI3;
#define LL long long

const int INF = 100000000;

vector<PI3> lines;
bool solved[1000];
int ta;
int sa, sb;
int cnt[2];

void solveIt(int be)
{
	solved[be]=true;
	int station = lines[be].second;
	int av = lines[be].first.second + ta;

	int i;
	FR(i, be+1, SZ(lines)-1)
		if (!solved[i] && lines[i].second != station && lines[i].first.first >= av)
		{
			solveIt(i);
			break;
		}
}

void process()
{
	cnt[0] = cnt[1] = 0;
	memset(solved, false, sizeof(solved));

	sort(ALL(lines));
	int i;
	RP(i, SZ(lines)) if (!solved[i])
	{
		cnt[lines[i].second]++;
		solveIt(i);
	}
}

int tomins(string s)
{
	int h = (s[0]-'0')*10 + (s[1]-'0');
	int m = (s[3]-'0')*10 + (s[4]-'0');
	return h*60+m;
}
 
int main()
{
	freopen(".\\B\\B-large.in", "r", stdin);
	freopen(".\\B\\B-large.out", "w", stdout);
	LL tc, i, j;
	cin >> tc;
	string x, y;
	RP(i, tc)
	{
		lines.clear();
		cin >> ta;
		cin >> sa >> sb;
		RP(j, sa)
		{
			cin >> x >> y;
			lines.pb(PI3(PII(tomins(x), tomins(y)), 0));
		}
		RP(j, sb)
		{
			cin >> x >> y;
			lines.pb(PI3(PII(tomins(x), tomins(y)), 1));
		}
		process();
		cout << "Case #" << (i+1) << ": " << cnt[0] << " " << cnt[1] << endl;
	}
	return 0;
}
