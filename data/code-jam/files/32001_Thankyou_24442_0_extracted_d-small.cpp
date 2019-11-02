#include <stdio.h>
#include <iostream>
#include <algorithm>
using namespace std;

int n;

string str;
string tmp;

int ans;

int len;

int p[10];

void dfs(int kth)
{
	if (kth == n+1)
	{
		bool ok = true;
		for (int i=1; i<=n; i++)
		{
			for (int j=i+1; j<=n; j++)
				if (p[i] == p[j])
				{
					ok = false;
					break;	
				}
			if (ok == false)
				break;
		}
		if (ok == true)
		{
			tmp = "";
			for (int i=0; i<len; i+=n)
			{
				for (int j=1; j<=n; j++)
				{
					tmp += str[i+p[j]-1];	
				}	
			}
			int key = 1;
			for (int i=1; i<len; i++)
				if (tmp[i] != tmp[i-1])
					key++;
					
			//cout << tmp << " " << key << endl;
			
			ans = min(ans, key);
		}
	}
	else{
		for (int i=1; i<=n; i++)
		{
			p[kth] = i;
			dfs(kth+1);	
		}
	}
}

int main()
{
	freopen("D-small.in", "r", stdin);
	freopen("D-small.out", "w", stdout);
	
	int cas;
	cin >> cas;
	for (int kth=1; kth<=cas; kth++)
	{
		cin >> n >> str;
		
		ans = len = str.length();
		
		dfs(1);
		
		cout << "Case #" << kth << ": " << ans << endl;	
	}	
	return 0;
}
