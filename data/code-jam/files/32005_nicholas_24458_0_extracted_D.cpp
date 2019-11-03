#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>


using namespace std;
const bool db = false;
int N, M;
int G1[10][10], G2[10][10];
int match[10];
bool used[10];



bool check()
{
	for (int i = 0; i < M; i++)
		for (int j = 0; j < M; j++)
			if (G2[i][j])
			{
				if (!G1[ match[i] ][ match[j] ]) return false;
			}

	if (db)
	{
		for (int i = 0; i < M; i++)
			cout<<match[i]<<' ';
		cout<<endl;
	}
	return true;
	

}
bool DFS(int x)
{
	if (x == M)
	{
		return check();
	}
	else
	{
		for (int i = 0; i < N; i++)
			if (!used[i])
			{
				used[i] = true;
				match[x] = i;
				if (DFS(x+1)) return true;
				used[i] = false;
			}
	}
	return false;
}

int main()
{
	int t;
//freopen("D-small-attemp0.in", "r", stdin);
	//freopen("D.out", "w", stdout);
	freopen("D.out", "w", stdout);
	cin>>t;
	for (int ctr = 1; ctr <= t; ctr++)
	{
		cin>>N;
		memset(G1, 0, sizeof(G1));
		memset(G2, 0, sizeof(G2));

		for (int i = 0; i < N-1; i++)
		{
			int x, y;
			cin>>x>>y;
			x--, y--;
			G1[x][y] = 1;
			G1[y][x] = 1;
		}

		cin>>M;
		for (int i = 0; i < M-1; i++)
		{
			int x, y;
			cin>>x>>y;
			x--, y--;
			G2[x][y] = 1;
			G2[y][x] = 1;
		}

		memset(used, false, sizeof(used));
		cout<<"Case #"<<ctr<<": ";
		if (DFS(0)) cout<<"YES"<<endl;
		else
			cout<<"NO"<<endl;
	}
	return 0;
}