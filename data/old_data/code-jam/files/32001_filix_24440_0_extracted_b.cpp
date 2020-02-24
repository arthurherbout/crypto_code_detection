#include<iostream>
#include<cstdio>
#include<cctype>
#include<cmath>
#include<cstdlib>
#include<algorithm>
#include<vector>
#include<string>
#include<list>
#include<deque>
#include<map>
#include<set>
#include<queue>
#include<stack>
#include<utility>
#include<sstream>
#include<cstring>
using namespace std;
 
typedef long long ll;
typedef vector<int> vi;
typedef pair<int,int> pii;

#define PB push_back
#define FORE(i,t) for(typeof(t.begin())i=t.begin();i!=t.end();++i)

int area(int x1,int y1,int x2,int y2,int x3,int y3)
{
	return abs(-x2*y1+x3*y1+x1*y2-x3*y2-x1*y3+x2*y3);
}

void solve(int x,int y,int a)
{
	for(int x2=0;x2<=x;++x2)
		for(int y2=0;y2<=y;++y2)
			for(int x3=0;x3<=x;++x3)
				for(int y3=0;y3<=y;++y3)
					if(area(0,0,x2,y2,x3,y3)==a)
					{
						printf("0 0 %d %d %d %d\n",x2,y2,x3,y3);
						return;
					}
	puts("IMPOSSIBLE");
}

int main()
{
	int Z;
	scanf("%d",&Z);
	for(int z=1;z<=Z;++z)
	{
		printf("Case #%d: ",z);
		int x,y,a;
		scanf("%d%d%d",&x,&y,&a);
		solve(x,y,a);
	}
}
