// a.cpp : Defines the entry point for the console application.
//

#include <iostream>
using namespace std;
#include <string>
#include <set>
#include <string.h>
#include <vector>
#include <algorithm>
#include <map>

map<int, set<int> > graf;
bool state[1005];
bool in[1005];

bool dfs(int i)
{
	if(state[i]) 
		return true;

	state[i] = true;

	for(set<int>::iterator I = graf[i].begin(); I!=graf[i].end(); I++)
		if(dfs(*I)) 
			return true;	
	return false;
}

void main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int T;
	cin>>T;

	for(int t=1; t<=T; t++)
	{
		int n;
		cin>>n;		
		graf.clear();
		for(int i=0; i<=n; i++)
		{
			state[i] = false;
			in[i] = false;
		}

		for(int i=1; i<=n; i++)
		{
			int m;
			cin>>m;			
			for(int j=0; j<m; j++)
			{
				int v;
				cin>>v;
				in[v] = true;
				graf[i].insert(v);
			}
			
		}
		bool yes = false;
		/*for(int i=1; i<=n && !yes; i++)
			if(!in[i])
			{
				for(int j=1; j<=n; j++)
					state[i] = false;
				if(dfs(i))
				{
					yes = true;
					break;
				}
			};*/
		for(int i=1; i<n && !yes; i++)
		{
			for(int j=1; j<=n; j++)
				state[j] = false;
			if(dfs(i))	yes = true;		
		}
		printf("Case #%d: ", t);
		if(yes) cout<<"Yes"<<endl;
		else cout<<"No"<<endl;
	}
}

