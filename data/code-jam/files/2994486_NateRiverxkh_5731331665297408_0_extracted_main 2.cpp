#include <cstdio>
#include <algorithm>
#include <cstring>
#include <vector>
#include <iostream>

using namespace std;

vector<int> v[100];
int zip[100], id[100], b[100];
int a[100];
string z[100];
string ans;
string cur;
bool chk( int* p, int N )
{
	int st[100], t = 0;
	st[++t] = p[0];
	cur="";
	for(int i=0;i<N;i++)
        cur+=z[p[i]];
	for( int i = 1; i < N; ++i )
	{
		int x = p[i];
		do
		{
			int y = st[t];
			bool ok = 0;
			for( int j = 0; !ok && j < v[y].size(); ++j )	if( v[y][j] == x )
				ok = 1;
			if( ok )
			{
				st[++t] = x;
				break;
			}
		} while(--t);
		if( t == 0 )	return 0;
	}
	return 1;
}

int main()
{
	freopen("C-small-attempt6.in", "r", stdin);
	freopen("C-small-attempt6.out", "w", stdout);

	int T, cases = 1;
	int N, M, x, y;

	scanf("%d", &T);
	while( T-- )
	{
		scanf("%d %d", &N, &M);
		for( int i = 0; i < N; ++i )
		{
			v[i].clear();
			cin>>z[i];
		}

		while( M-- )
		{
			scanf("%d %d", &x, &y);
			x--; y--;
			v[x].push_back(y);
			v[y].push_back(x);
		}
		ans="";
		for(int i=0;i<N;i++) a[i]=i;
		do
		{
			for( int i = 0; i < N; ++i )	b[i] = a[i];
			if( chk(b, N) )
			{
				if(ans=="") ans=cur;
				else if(cur<ans) ans=cur;
			}
		} while( next_permutation(a, a+N) );
		printf("Case #%d: ",cases++);
		cout<<ans<<endl;
	}

	return 0;
}
