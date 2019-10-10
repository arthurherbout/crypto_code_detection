// B.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct Node
{
	int x, y;
	long long dist;
	Node (int _x, int _y, long long _dist): x(_x), y(_y), dist(_dist)
	{
	}
	Node()
	{
	}
};

struct Light
{
	int s, w, t;
};

queue<Node> qu;
long long mp[60][60];
Light light[30][30];
int n, m;
int main()
{
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
	void solve(int i );
	int t;
	cin>>t;
	for(int i = 0;i<t;i++)
	{
		solve(i);
	}

	return 0;
}

void solve(int cas)
{
	void bfs();
	cin>>n>>m;

	for(int i = 0;i<2 * n + 2;i++)
	{
		for(int j = 0;j<2 * m + 2;j++)
		{
			mp[i][j] = 1000000000000;
		}
	}
	memset(light, 0, sizeof(light));

	for(int i = 0;i<n;i++)
	{
		for(int j = 0;j<m;j++)
		{
			cin>>light[i][j].s >> light[i][j].w >> light[i][j].t;
		}
	}

	//cout<<"read done"<<endl;
	mp[n * 2 -1][0] = 0;
	qu.push(Node(n * 2-1, 0, 0));
	bfs();
	printf("Case #%d: %lld\n",cas + 1,  mp[0][2 * m -1]);
	//cout<<mp[0][2 * m -1]<< endl;
}

void bfs()
{
	long long up(long long, int, int);
	long long down(long long, int, int);
	long long left(long long, int, int);
	long long right(long long, int, int);
	Node tmp;

	//cout<<"bfs"<<endl;
	while(!qu.empty())
	{
		tmp = qu.front();
		qu.pop();

		//cout << "pop: "<<tmp.x << " "<< tmp.y << " "<< tmp.dist<<endl;
		long long over;
		over = up(tmp.dist, tmp.x, tmp.y);
		if(tmp.x > 0 && over < mp[tmp.x - 1][tmp.y])
		{
			mp[tmp.x - 1][tmp.y] = over;
			qu.push(Node(tmp.x - 1, tmp.y, over));
		//	cout << tmp.x - 1 << " " << tmp.y << " " <<over << endl;
		}

		over = down(tmp.dist, tmp.x, tmp.y);
		if(tmp.x < 2 * n -1 && over < mp[tmp.x + 1][tmp.y])
		{
			mp[tmp.x + 1][tmp.y] = over;
			qu.push(Node(tmp.x + 1, tmp.y, over));
		//	cout << tmp.x + 1 << " " << tmp.y << " " <<over << endl;
		}

		over = left(tmp.dist, tmp.x, tmp.y);
		if(tmp.y > 0 && over < mp[tmp.x][tmp.y -1])
		{
			mp[tmp.x][tmp.y - 1] = over;
			qu.push(Node(tmp.x, tmp.y - 1, over));
		//	cout << tmp.x << " " << tmp.y -1 << " " <<over << endl;
		}

		over = right(tmp.dist, tmp.x, tmp.y);
			if(tmp.y < 2 * m-1 && over< mp[tmp.x][tmp.y + 1])
		{
			mp[tmp.x][tmp.y + 1] = over;
			qu.push(Node(tmp.x, tmp.y + 1, over));
		//	cout << tmp.x << " " << tmp.y + 1 << " " <<over << endl;
		}
	}
}

long long up(long long time, int i, int j)
{
	if(i <= 0)
	{
		return 1000000000000;
	}
	if((i & 1) == 0)
		return time + 2;
	int s = light[i>>1][j>>1].s;
	int w = light[i>>1][j>>1].w;
	int t = light[i>>1][j>>1].t;

	long long tmp = (time + (s + w) * 10000000000 - t)%(s + w);

	if(tmp < s)
	{
		return time + 1;
	}
	else
	{
		return time + s + w + 1 - tmp;
	}
}

long long down(long long time, int i, int j)
{
	if(i >= n * 2 + 1)
		return 1000000000000;
	if((i & 1) == 1)
	{
		return time + 2;
	}

	int s = light[i>>1][j>>1].s;
	int w = light[i>>1][j>>1].w;
	int t = light[i>>1][j>>1].t;

	long long tmp = (time + (s + w) * 10000000000 - t)%(s + w);

	if(tmp < s)
	{
		return time + 1;
	}
	else
	{
		return time + s + w + 1 - tmp;
	}
}

long long left(long long time, int i, int j)
{
	if(j <= 0)
		return 1000000000000;
	if((j & 1) == 0)
	{
		return time + 2;
	}

	int s = light[i>>1][j>>1].s;
	int w = light[i>>1][j>>1].w;
	int t = light[i>>1][j>>1].t;

	long long tmp = (time + (s + w) * 10000000000 - t)%(s + w);

	if(tmp >= s)
	{
		return time + 1;
	}
	else
	{
		return time + s - tmp + 1;
	}
}

long long right(long long time, int i, int j)
{
	if(j >= m * 2 + 1)
		return 1000000000000;
	if((j & 1) == 1)
	{
		return time + 2;
	}

	int s = light[i>>1][j>>1].s;
	int w = light[i>>1][j>>1].w;
	int t = light[i>>1][j>>1].t;

	long long tmp = (time + (s + w) * 10000000000 - t)%(s + w);

	if(tmp >= s)
	{
		return time + 1;
	}
	else
	{
		return time + s - tmp + 1;
	}
}