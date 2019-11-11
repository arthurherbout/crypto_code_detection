#include<stdio.h>
#include<vector>
#include<string.h>
#include<string>
#include<iostream>
using namespace std;
#define MAX 110
vector<string> root[10000];

int n, m;

void readin()
{
	char in[MAX], *p;
	root[0].clear();

	scanf("%d %d\n", &n, &m);
	for(int i = 1; i <= n+m; i++)
	{
		root[i].clear();
		gets(in);
		p = strtok(in, "/");
		while(p)
		{
			string s;
			s.append(p);
			root[i].push_back(s);
			p = strtok(NULL, "/");
		}

	}
	n++;

}

int cmp(int p, int q)
{
	int i;
	if(root[p].size() < root[q].size())
	{
		for(i = 0 ; i < root[p].size(); i++)
			if(root[p][i].compare(root[q][i]) != 0) break;

	}
	else
	{
		for(i = 0 ; i < root[q].size(); i++)
			if(root[p][i].compare(root[q][i]) != 0) break;

	}
	return root[p].size() - i;


}

void solve()
{
	int tmp, c, ans = 0, pt = 0;
	for(int i = n; i < n+m; i++, pt++)
	{
		c = 100000;
		for(int j = 0; j < n+pt; j++)
		{
			tmp = cmp(i, j);
			if(tmp < c) c = tmp;
		}
		ans+=c;
	}
	printf("%d\n", ans);

}
int main()
{
#ifndef ONLINE_JUDGE
	//freopen("data.txt","r",stdin);
	freopen("A-large.in","r",stdin);
	freopen("A-large.out","w",stdout);
#endif

	int cases;
	scanf("%d\n", &cases);

	for(int i = 1; i <= cases; i++)
	{
		printf("Case #%d: ", i);
		readin();
		solve();
	}

	return 0;

}
