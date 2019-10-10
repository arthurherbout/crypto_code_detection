#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define max(a,b) ((a)>(b)?(a):(b))
struct Node
{
	int x,y ,r;
}pt[100];
int n;
void solve()
{
	int i,j,k;
	double len[0],ans;
	scanf("%d",&n);
	for(i=0;i<n;++i)
		scanf("%d%d%d",&pt[i].x, &pt[i].y,&pt[i].r);
	ans= 1000000000;
	if(n==1)
		ans= double(pt[0].r);
	else if(n==2)
		ans= max(double(pt[0].r),double(pt[1].r));
	else
	{
		for(i=0;i<n;++i)
		{
			len[0]=(double)pt[i].r;
			len[1]=((double)pt[(i+1)%n].r + pt[(i+2)%n].r + sqrt((double)(pt[(i+1)%n].x-pt[(i+2)%n].x)*(pt[(i+1)%n].x-pt[(i+2)%n].x) + (double)(pt[(i+1)%n].y-pt[(i+2)%n].y)*(pt[(i+1)%n].y-pt[(i+2)%n].y)))/2.0;
			len[0]=max(len[0],len[1]);
			if(len[0] < ans)
				ans=len[0];
		}
	}
	printf("%.7lf\n",ans);
}
int main()
{
	freopen("D-small-attempt0.in","r",stdin);
	freopen("ddd.out","w",stdout);
	int T, i;
	scanf("%d",&T);
	for(i=1;i<=T;i++)
	{
		printf("Case #%d: ",i);
		solve();
	}
	return 0;
}



