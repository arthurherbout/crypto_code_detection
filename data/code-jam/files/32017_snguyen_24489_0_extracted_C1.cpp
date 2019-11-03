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
#define SZ(a) (LL)a.size()
#define ALL(a) a.begin(), a.end()
#define pb push_back
typedef vector <int> VI;
typedef vector <string> VS;
typedef pair<int, int> PII;
#define LL long long

int n;
int K;
int ld;
VI dd;
int val[5555];

void process()
{
	VI board;
	int i, cnt, off=0;
	RP(i, K) board.pb(i);
	memset(val, -1, sizeof(val));

	FR(cnt, 0, K-1)
	{
		int k = cnt % SZ(board);
		k = (k + off) % SZ(board);
		val[board[k]]=cnt+1;
		//VI buf = board;
		//board.clear();
		//FR(i, k+1, SZ(buf)-1) board.pb(buf[i]);
		//FR(i, 0, k-1) board.pb(buf[i]);
		board.erase(board.begin()+k, board.begin()+k+1);
		if (SZ(board)>0) off = (k) % SZ(board);
	}
}

int main()
{
	//freopen("Round 1B\\C-sample.in", "r", stdin);
	//freopen("Round 1B\\C-sample.out", "w", stdout);
	freopen("Round 1B\\C-small-attempt1.in", "r", stdin);
	freopen("Round 1B\\C-small-attempt1.out", "w", stdout);
	LL numtest, test, i;
	cin >> numtest;
	RP(test, numtest)
	{
		cin >> K;
		cin >> ld;
		dd.resize(ld);
		RP(i, ld) cin >> dd[i];
		process();
		cout << "Case #" << (test+1) << ": ";
		RP(i, ld)
		{
			cout << val[dd[i]-1];
			if (i < ld-1) cout << " ";
		}
		cout << endl;
	}
	return 0;
}
