#pragma comment(linker, "/STACK:134217728")

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>

#include <vector>
#include <string>
#include <set>
#include <map>
#include <queue>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

#define sqr(a) ((a)*(a))
#define det2(a,b,c,d) ((a)*(d) - (b)*(c))

#define FOR(i,a,b) for(i=(a); i<(b); ++i)

int a[310], b[310];
char c[310][15];

int main()
{
    int i, j;
    int TST, cas;
	int res;
	int n;
    scanf( "%d", &TST );
    for ( cas = 1 ; cas <= TST ; cas++ )
    {
		scanf("%d", &n);
		FOR(i,0,n) scanf("%s%d%d", &c[i], &a[i], &b[i]);

		res = 1000000;

		for(int m=0;m<(1<<n);m++)
		{
			set<string> cols;
			vector<bool> used(10001, 0);
			int bts=0;
			FOR(i,0,n) if(m & (1<<i))
			{
				bts++;
				cols.insert(c[i]);
				for(j=a[i];j<=b[i];j++)
					used[j]=1;
			}
			if(cols.size() <= 3)
			{
				for(i=1;i<=10000;i++) if(used[i] == 0) break;
				if(i>10000)
				{
					res = min(res, bts);
				}
			}
		}



        printf( "Case #%d: ", cas);
		if(res == 1000000)
			printf( "IMPOSSIBLE\n");
		else
			printf( "%d\n", res);
    }


    return 0;
}

