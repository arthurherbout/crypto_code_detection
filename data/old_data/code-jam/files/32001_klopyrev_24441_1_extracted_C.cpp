#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <string>
using namespace std;

#define FR(i,a,n) for(int (i) = (a); (i)<(n); (i)++)
#define RF(i,a,n) for(int (i) = int(n)-1; (i)>=(a); (i)--)
#define FOR(i,n) FR(i,0,n)
#define ROF(i,n) RF(i,0,n)

typedef long long ll;
typedef long double ld;
typedef pair<int,int> pii;
typedef pair<ll,ll> pll;
typedef vector<int> vi;

int dr[4] = {-1,0,1,0};
int dc[4] = {0,1,0,-1};
int coor[1000][4];
int N;
int p[1000];
ld cr[4][2];
bool check(ld v)
{
	FOR(i,4)
	{
		cr[i][0] = -1e19;
		cr[i][1] = 1e19;
	}
	FOR(i,N)
	{
		ld diff = p[i]*v;
		FOR(j,4)
		{
			cr[j][0] = max(cr[j][0], coor[i][j]-diff);
			cr[j][1] = min(cr[j][1], coor[i][j]+diff);
		}
	}
	FOR(i,4)
		if(cr[i][0]>cr[i][1])
			return false;
	return true;
}


int main()
{
	int TESTS;
	scanf("%d", &TESTS);
	FOR(tests, TESTS)
	{
		scanf("%d", &N);
		FOR(i,N)
		{
			int x, y, z;
			scanf("%d%d%d%d", &x, &y, &z, p+i);
			coor[i][0] = x+y+z;
			coor[i][1] = x+y-z;
			coor[i][2] = x-y+z;
			coor[i][3] = x-y-z;
		}
		ld low = 0;
		ld high = 1e18;
		while((high-low)>1e-9 && (high-low)/low>1e-9)
		{
			ld mid = (low+high)/2;
			if(check(mid))
				high = mid;
			else
				low = mid;
		}
		printf("Case #%d: %.9Lf\n", tests+1, low);
	}
	return 0;
}
