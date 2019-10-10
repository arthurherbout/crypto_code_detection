#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<limits>
#include<cmath>
#include<cstring>
#include<queue>
#include<algorithm>
#include<stack>
#include<map>
#include<vector>
using namespace std;

#define rep(i,n) for(int i=0; i<(n); ++i)
#define repf(i,a,b) for(int i=(a); i<=(b); ++i)
#define repd(i,a,b) for(int i=(a); i>=(b); --i)
#define ll long long
#define PB(i) push_back(i)
#define MP make_pair
#define N 1000
int test,n;
int a[N+10];
int b[N+10];
int point[N+10];
int pre[N+10];
struct node{
	int x,y;
	node(){}
	node(int _x,int _y):x(_x),y(_y){}
};
node p[N+10];
int main()
{
	freopen("in.in","r",stdin);
	freopen("out","w",stdout);
	srand(time(0));
	scanf("%d",&test);
	repf(ror,1,test){
		scanf("%d",&n);
		rep(i,n)
			scanf("%d",&a[i]),b[i]=i,
			point[i]=i;
		rep(i,n)
		{
			int x=i;
			int y=point[a[i]];
			swap(b[x],b[y]);
			point[b[x]]=x;
			point[b[y]]=y;
			p[i].x=x; p[i].y=y;
		}
        repd(i,n-1,0)
		{
			swap(a[p[i].x],a[p[i].y]);
		}
		int flag=0;
		rep(i,n)
			if(a[i]!=i)
				flag=1;
		printf("Case #%d: ",ror);
		if(flag==0 && rand()%4!=1 ) printf("GOOD\n");
		else printf("BAD\n");
	}
	return 0;
}
