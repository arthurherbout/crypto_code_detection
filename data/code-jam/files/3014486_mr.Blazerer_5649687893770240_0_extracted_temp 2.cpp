#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory.h>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <list>
#include <stack>
#include <set>
#include <map>

using namespace std;
int res;
int cnt;
struct st
{
	st* to[26];
	int cnt;
	st()
	{
		for (int i=0;i<26;i++)
			to[i]=NULL;
		cnt = 0;
	}
};

int n,m;
string str[8];
st* trees[4];
int curcnt[4];
void addto(const string& str, int idx)
{
	st* cur = trees[idx];
	cur->cnt++;
	for (int i=0;i<str.size();i++)
	{
		int c = str[i]-'A';
		if (cur->to[c] == NULL)
		{
			cur->to[c] = new st;
			curcnt[idx]++;
		}

		cur = cur->to[c];
		cur->cnt++;
	}
}
void releasefrom(const string& str, int idx)
{
	stack<st*> order;
	stack<int> chars;
	st* cur = trees[idx];
	cur->cnt--;
	order.push(cur);
	for (int i=0;i<str.size();i++)
	{
		int c = str[i]-'A';
		cur = cur->to[c];
		order.push(cur);
		chars.push(c);
	}
	while (!chars.empty())
	{
		int c = chars.top();
		chars.pop();
		st* cur = order.top();
		order.pop();
		cur->cnt--;
		if (cur->cnt==0)
		{
			delete cur;
			curcnt[idx]--;
			(order.top())->to[c] = NULL;
		}
	}
}
int getres()
{
	int sum =0;
	for (int i=0;i<n;i++)
		sum+=curcnt[i];
	return sum;
}
void dfs(int step)
{
	if (step==m)
	{
		for (int i=0;i<n;i++)
		{
			if (trees[i]->cnt == 0)
				return;
		}
		int t = getres();
		if (t>res)
		{
			res = t;
			cnt = 1;
		}
		else
		if (t==res)
			cnt++;
		return ;
	}
	for (int i=0;i<n;i++)
	{
		addto(str[step],i);
		dfs(step+1);
		releasefrom(str[step],i);
	}
}
void go()
{
	cin>>m>>n;
	for (int i=0;i<m;i++)
		cin>>str[i];
	res = 0;
	cnt = 0;
	for (int i=0;i<n;i++)
	{
		if (trees[i]!=NULL)
		{
			delete trees[i];
		}
		trees[i] = new st;
		curcnt[i]=1;
	}
	dfs(0);
	cout<<res<<' '<<cnt;
}
int main()
{	
	freopen("1.txt","r",stdin);
	freopen("2.txt","w",stdout);
	int cases;
	cin>>cases;
	for (int curcase=1;curcase<=cases;curcase++)
	{
		cout<<"Case #"<<curcase<<": ";
		{
			go();
		}
		cout<<"\n";
	}
	return 0;
}
