#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <memory.h>

using namespace std;

#define pb push_back
#define mp make_pair

typedef long long lint;
typedef unsigned long long ull;

const int INF = 1000000000;
const lint LINF = 1000000000000000000ll;
const double eps = 1e-8;

int n;
int pre[105];
char c[105];
int m;
vector< string> cool;

vector< pair< lint, lint> > mc(int iterations, int rrr)
{
	srand(rrr);

	vector< pair< lint, lint> > ans;

	for (int i = 0; i < m; i++)
	{
		ans.push_back(mp(0, 0));
	}

	set< vector<int> > seqs;

	string word;
	vector< int> seq;
	word.resize(n, ' ');
	seq.resize(n, -1);
	int q[105], qs, qe;
	while (iterations--)
	{
		qs = 0;
		qe = 0;
		for (int i = 0; i < n; i++)
			if (pre[i] == -1)
			{
				q[qe++] = i;
				if (qe == n + 1)
					qe = 0;
			}

		for (int k = 0; k < n; k++)
		{
			if (qs == qe)
			{
				printf("FUUU\n");
			}

			int id = 0, len = 0;
			if (qs < qe)
			{
				len = (qe - qs);
				id = qs + (rand() % len);
			}
			else
			{
				len = (n - qs + qe);
				id = rand() % len;
				if (id >= qe)
					id += qs - qe;
			}

			swap(q[qs], q[id]);
			id = q[qs];

			for (int i = 0; i < n; i++)
				if (pre[i] == id)
				{
					q[qe++] = i;
					if (qe == n + 1)
						qe = 0;
				}

			seq[k] = id;
			word[k] = c[id];

			qs++;
			if (qs == n + 1)
				qs = 0;
		}

		if (qs != qe)
		{
			printf("FUUU\n");
		}

		if (seqs.find(seq) != seqs.end())
		{
			continue;
		}

		seqs.insert(seq);

		for (int i = 0; i < m; i++)
		{
			if (word.find(cool[i]) != string::npos)
			{
				ans[i].first++;
			}
			ans[i].second++;
		}
	}

	fprintf(stderr, "seqs = %d\n", seqs.size());

	return ans;
}

void solve()
{
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
	{
		scanf("%d", &pre[i]);
		pre[i]--;
	}
	scanf("%s", c);

	cool.clear();
	scanf("%d", &m);
	char tmp[105];
	for (int i = 0; i < m; i++)
	{
		scanf("%s", tmp);
		cool.push_back(string(tmp));
	}

	vector< pair< lint, lint> > ansp[8];
	
#pragma omp parallel for
	for (int i = 0; i < 8; i++)
		ansp[i] = mc(3000, i);

	vector< pair< lint, lint> > ans;
	ans = ansp[0];
	for (int i = 1; i < 8; i++)
		for (int j = 0; j < ans.size(); j++)
		{
			ans[j].first += ansp[i][j].first;
			ans[j].second += ansp[i][j].second;
		}

	double v = ans[0].second;
	printf("%.2lf", ans[0].first / v);
	for (int i = 1; i < m; i++)
		printf(" %.2lf", ans[i].first / v);

	printf("\n");
}

int main()
{
	omp_set_num_threads(8);

	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tn;
	scanf("%d", &tn);
	for (int i = 0; i < tn; i++)
	{
		fprintf(stderr, "Test #%d\n", i + 1);
		printf("Case #%d: ", i + 1);
		solve();
	}

	return 0;
}
