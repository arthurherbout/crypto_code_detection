#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <queue>
#include <list>
#include <map>

using namespace std;

int s2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384};
char cc[10000][15];
char chrs[30];
list<int> grp[10000];
queue<int> Q;
list<int> l;
int pts[10000];
int n, m;

int main()
{
	int t, cases = 0;
	scanf("%d", &t);
	while (t--)
	{
		scanf("%d%d", &n, &m);
		for (int i = 0; i < n; ++i)
			scanf("%s", cc[i]);

		printf("Case #%d:", ++cases);
		for (int i = 0; i < m; ++i)
		{
			scanf("%s", chrs);

			for (int q = 0; q < n; ++q)
			{
				int l = strlen(cc[q]);
				grp[l].push_back(q);
			}
			for (int q = 0; q < 10000; ++q)
			{
				if (grp[q].empty()) Q.push(q);
				else l.push_back(q);
			}
			fill(pts, pts + n, 0);

			for (int j = 0; j < 26; ++j)
			{
				vector<int> cands;
				for (list<int>::iterator li = l.begin(); li != l.end();)
				{
					int p = *li;
					if (grp[p].size() < 2)
					{
						grp[p].clear();
						Q.push(p);
						li = l.erase(li);
						continue;
					}
					map<int, int> m;
					vector<int> v;
					for (list<int>::iterator lj = grp[p].begin(); lj != grp[p].end();)
					{
						int x = 0, w = *lj;
						for (int q = 0; cc[w][q] != 0; ++q)
						{
							if (cc[w][q] == chrs[j])
								x |= s2[q];
						}
						if (m.empty())
							m.insert(make_pair(x, p));

						int nex = p;
						map<int, int>::iterator mi = m.find(x);
						if (mi == m.end())
						{
							nex = Q.front();
							m.insert(make_pair(x, nex));
							Q.pop();
						}
						else
							nex = mi->second;

						if (x == 0)
							v.push_back(w);
						if (nex != p)
						{
							grp[nex].push_back(w);
							cands.push_back(nex);
							lj = grp[p].erase(lj);
							continue;
						}
						++lj;
					}
					if (m.size() > 1)
					{
						for (int pp = 0; pp < v.size(); ++pp)
						{
							pts[v[pp]]++;
						}
					}

					++li;
				}
				for (int pp = 0; pp < cands.size(); ++pp)
					l.push_back(cands[pp]);
			}

			int pt = -1, ps = -1;
//			fprintf(stderr, "\n");
			for (int j = 0; j < n; ++j)
			{
				if (pts[j] > pt)
				{
					pt = pts[j];
					ps = j;
				}
				//fprintf(stderr, "pts[%d]: %d\n", j, pts[j]);
			}
			printf(" %s", cc[ps]);

			for (int i = 0; i < 10000; ++i)
				grp[i].clear();
			while (!Q.empty()) Q.pop();
			l.clear();
			//fprintf(stderr, "\n");
		}
		printf("\n");
	}
}