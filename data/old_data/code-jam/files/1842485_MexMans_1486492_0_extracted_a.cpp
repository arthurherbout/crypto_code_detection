#include<iostream>
#include<stdio.h>
#include<assert.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include<string>
#include<sstream>
#include<map>
#include<set>
#include<queue>
#include<stack>
#include<vector>
#include<algorithm>
#pragma comment(linker, "/STACK:16777216")
#define pb push_back
#define ppb pop_back
#define mp make_pair
#define all(x) (x).begin(),(x).end()
#define sz(x) (int)(x).size()
#define LL long long
#define bit __builtin_popcountll
#define sqr(x) (x) * (x)
using namespace std;
typedef pair<int, int> pii;
const double eps = 1e-9;
const double pi = acos(-1.0);
int a[2200];
bool left(int x1, int y1, int x2, int y2, int x3, int y3)
{
	return (LL)(x2 - x1) * (y3 - y1) - (LL)(y2 - y1) * (x3 - x1) > 0;
}
int main()
{
	#ifndef ONLINE_JUDGE
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	#endif
	int T,n;
	scanf("%d",&T);
	for(int tt = 0; tt < T; tt++)
	{
		scanf("%d",&n);
		for(int i = 0; i < n - 1; i++)
		{
			scanf("%d",&a[i]);
			--a[i];
		}
		vector<int> v(n);
		for(int i = 0; i < n; i++)
			v[i] = i;
		bool finded = false;
		do
		{
			bool ok = true;
			for(int i = 0; i < n - 1; i++)
			{
				int who = i + 1;
				for(int j = i + 2; j < n; j++)
					if (left(i,v[i],who,v[who],j,v[j]))
						who = j;
				if (who != a[i])
				{
					ok = false;
					break;
				}
			}
			if (ok) 
			{
				finded = true;
				break;
			}
		} while( next_permutation(all(v)) );
		printf("Case #%d: ",tt + 1);
		if (!finded) puts("Impossible"); else
		{
			for(int i = 0; i < n; i++)
			{
				if (i) printf(" ");
				printf("%d",v[i]);
			}
			printf("\n");
		}		
	}
	return 0;
}
