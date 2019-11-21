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
int p2[maxn];
set<int> wasset;
vector<int> gr[maxn];
int n;

bool go(int cur)
{
	if (was[cur] == wast) return false;
	was[cur] = wast;
// 	cout << "go " << cur << endl;
	for (auto to : gr[cur])
	{
		if (p[to] == -1 || go(p[to]))
		{
			p[to] = cur;
			p2[cur] = to;
			return true;
		}
	}
	return false;
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
				for (int i = pwas + 1; i <= last; i++)
				{
					p[p2[i]] = -1;
					p2[i] = -1;
				}
				curans = 0;
				pwas = t - 1;
			}
			while (true)
			{
				wast++;
				if (go(t))
				{
					curans++;
					break;
				}
				pwas++;
// 				cout << "delete " << pwas << endl;
				p[p2[pwas]] = -1;
				p2[pwas] = -1;
				curans--;
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
