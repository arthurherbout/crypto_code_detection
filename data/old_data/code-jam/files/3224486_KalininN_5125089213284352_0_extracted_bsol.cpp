#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ld = long double;
using D = double;
using uint = unsigned int;

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

using tans = pair<int, ld>;

const int maxn = 105;
const int MAX_IT = 10000;

vector<int> roots, roots2;
vector<int> gr[maxn];
tans ans[maxn];
int n, m;
char s[maxn], curs[maxn];
char t[10][30];
ld c[maxn][maxn];
int kv[10];

tans merge(tans a, tans b)
{
	return {a.fi + b.fi, c[a.fi + b.fi][a.fi] * a.se * b.se};
}

void calc(int cur)
{
	ans[cur] = {0, 1};
	for (auto t : gr[cur])
	{
		calc(t);
		ans[cur] = merge(ans[cur], ans[t]);
	}
	ans[cur].fi++;
}

void go(int cur, vector<int> &curroots)
{
	if (cur == n) return;
	int n = curroots.size();
	vector<ld> prob(n);
	vector<tans> back(n + 1);
	vector<int> will = curroots;
	tans nowback = {0, 1};
	back[n] = nowback;
	for (int i = n - 1; i >= 0; i--)
	{
		nowback = merge(ans[curroots[i]], nowback);
		back[i] = nowback;
	}
	tans nowfwd = {0, 1};
	ld sum = 0;
	for (int i = 0; i < n; i++)
	{
		tans now = merge(nowfwd, back[i + 1]);
		for (auto t : gr[curroots[i]]) now = merge(now, ans[t]);
		prob[i] = now.se;
		sum += now.se;
		nowfwd = merge(ans[curroots[i]], nowfwd);
	}
	for (int i = 0; i < n; i++) prob[i] /= sum;
	ld r = (ld)rand() / RAND_MAX;
	sum = 0;
	int choice = n - 1;
	for (int i = 0; i < n; i++)
	{
		sum += prob[i];
		if (sum > r)
		{
			choice = i;
			break;
		}
	}
	swap(curroots.back(), curroots[choice]);
	int v = curroots.back();
	curroots.pop_back();
	curs[cur] = s[v];
	for (auto t : gr[v]) curroots.pb(t);
	go(cur + 1, curroots);
}

int main()
{
	c[0][0] = 1;
	for (int i = 1; i <= 100; i++)
	{
		c[i][0] = 1;
		for (int j = 1; j <= i; j++) c[i][j] = c[i - 1][j - 1] + c[i - 1][j];
	}
	
    int NT = 0;
    scanf("%d", &NT);
    for (int T = 1; T <= NT; T++)
    {
        printf("Case #%d:", T);
        
		
		scanf("%d", &n);
		for (int i = 0; i < n; i++) gr[i].resize(0);
		roots.clear();
		for (int i = 0; i < n; i++)
		{
			int x;
			scanf("%d", &x);
			if (x == 0) roots.pb(i);
			else gr[x - 1].pb(i);
		}
		scanf("%s", s);
		scanf("%d", &m);
		for (int i = 0; i < m; i++) scanf("%s", t[i]);
		for (auto t : roots) calc(t);
		for (int i = 0; i < m; i++) kv[i] = 0;
		for (int IT = 0; IT < MAX_IT; IT++)
		{
			roots2 = roots;
			go(0, roots2);
			curs[n] = '\0';
			for (int i = 0; i < m; i++)
			{
				kv[i] += strstr(curs, t[i]) != NULL;
			}
		}
		for (int i = 0; i < m; i++) printf(" %.4lf", (double)kv[i] / MAX_IT);
		printf("\n");
		
        fprintf(stderr, "%d/%d cases done! %.2f\n", T, NT, (double)clock() / CLOCKS_PER_SEC);
    }
    return 0;
}
