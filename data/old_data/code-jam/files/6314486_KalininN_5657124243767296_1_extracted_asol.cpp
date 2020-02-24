#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ld = long double;
using D = double;
using uint = unsigned int;
template<typename T>
using pair2 = pair<T, T>;

#ifdef WIN32
    #define LLD "%I64d"
#else
    #define LLD "%lld"
#endif

#define pb push_back
#define mp make_pair
#define all(x) (x).begin(),(x).end()
#define fi first
#define se second

const int inf = 1e9;
const int maxn = 1000006;

int was[maxn];
int pwas, wast, curtime;
int p[maxn];
set<int> wasset;
vector<int> gr[maxn];
int n;

int go(int cur)
{
	if (was[cur] == wast) return inf;
	was[cur] = wast;
// 	cout << "go " << cur << endl;
	int retmin = inf;
	for (auto to : gr[cur])
	{
		if (p[to] <= pwas)
		{
			p[to] = cur;
			return -1;
		}
		retmin = min(retmin, p[to]);
		auto t = go(p[to]);
		if (t <= pwas)
		{
// 			cout << to << " to " << cur << ' ' << t << endl;
			p[to] = cur;
			return -1;
		} else retmin = min(retmin, t);
	}
	return retmin;
}

int main()
{
    int NT = 0;
    scanf("%d", &NT);
    for (int T = 1; T <= NT; T++)
    {
        printf("Case #%d: ", T);
        
		scanf("%d", &n);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				int x;
				scanf("%d", &x);
				x--;
				wasset.insert(x);
				gr[x].pb(i);
			}
			p[i] = -1;
		}
		
		int last = -1;
		pwas = -1;
		int curans = 0;
		int answer = 0;
		wast = 1;
		for (auto t : wasset)
		{
// 			cout << "check " << t << endl;
			wast++;
			curtime = t;
			if (t != last + 1)
			{
				pwas = t - 1;
				curans = 0;
			}
			auto makelast = go(t);
			if (makelast <= pwas)
			{
// 				cout << "ok " << endl;
				curans++;
			} else
			{
// 				cout << "need remove " << makelast << endl;
// 				while (true)
// 				{
// 					pwas++;
// 					curans--;
// 					wast++;
// 					if (go(t) <= pwas)
// 					{
// 						curans++;
// 						break;
// 					}
// 				}
				curans -= makelast - pwas;
				pwas = makelast;
				wast++;
				assert(go(t) <= pwas);
				curans++;
			}
			answer = max(answer, curans);
			last = t;
// 			cout << "answer is " << curans << endl;
		}
		assert(answer <= n);
		printf("%d\n", answer);
		
		for (auto t : wasset)
		{
			gr[t].clear();
			was[t] = -1;
		}
		wast = 0;
		wasset.clear();

        fprintf(stderr, "%d/%d cases done!\n", T, NT);
    }
    return 0;
}
