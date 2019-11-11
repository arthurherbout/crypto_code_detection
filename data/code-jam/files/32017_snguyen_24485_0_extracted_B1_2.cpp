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
int A, B, P;
VI pp;
int belong[2002];
int ex[2002];
int ans;

bool isPrime(int v)
{
	int i;
	for (i=2; i*i<=v; i++)
		if (v%i==0) return false;
	return true;
}

void process()
{
	int i, j, k, h;
	FR(i, A, B) FR(j, i+1, B) if (belong[i]!=belong[j])
	{
		RP(k, SZ(pp)) if (i%pp[k]==0 && j%pp[k]==0)
		{
			int v=belong[j];
			FR(h, A, B) if (belong[h]==v) belong[h]=belong[i];
			break;
		}
	}
}

void out()
{
	ans=0;
	memset(ex, false, sizeof(ex));
	int i;
	FR(i, A, B) if (!ex[belong[i]])
	{
		ex[belong[i]]=true;
		ans++;
	}
}

int main()
{
	//freopen("Round 1B\\B-sample.in", "r", stdin);
	freopen("Round 1B\\B-small-attempt2.in", "r", stdin);
	freopen("Round 1B\\B-small-attempt2.out", "w", stdout);
	LL numtest, test, i;
	cin >> numtest;
	RP(test, numtest)
	{
		cin >> A >> B >> P;
		pp.clear();
		FR(i, P, B) if (isPrime(i)) pp.pb(i);
		memset(belong, 0, sizeof(belong));
		int cnt=0;
		FR(i, A, B) belong[i] = cnt++;
		process();
		out();
		cout << "Case #" << (test+1) << ": " << ans << endl;
	}
	return 0;
}
