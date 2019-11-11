#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
#define nNode 1000006
double X,S,R,N,T;
double sum;
struct node
{
	double st,ed;
	double wi;
	double v;
	void input()
	{
		scanf("%lf%lf%lf",&st,&ed,&wi);
		v=wi+S;
		sum=sum+ed-st;
	}
};

int cmp(node a,node b)
{
	return a.v<b.v;
}

node ans[nNode];
void slove()
{
	double dans=X-sum;
	double t=dans/R;
	double anst=0;
	if(t<T)
	{
		anst=anst+t;
		T-=t;
	}
	else
	{
		double s=T*R;
		anst=anst+T;
		t=(dans-s)/S;
		anst=anst+t;
		T=0;
	}
	int i=0;
	while(T>0 && i<N)
	{
		if((ans[i].v+R-S)>0)
		{
			t=(ans[i].ed-ans[i].st)/(ans[i].v+R-S);
			if(t<T)
			{
				anst=anst+t;
				T-=t;
			}
			else
			{
				double s=T*(ans[i].v+R-S);
				anst=anst+T;
				t=(ans[i].ed-ans[i].st-s)/ans[i].v;
				anst=anst+t;
				T=0;
			}
		}
		else
		{
			break;
		}
		i++;
	}
	for(;i<N;i++)
	{
		t=(ans[i].ed-ans[i].st)/ans[i].v;
		anst=anst+t;
	}
	printf("%.6lf\n",anst);
}
int main()
{
	int t;
	int i;
	int Case=0;
	freopen("A_in.in","r",stdin);
	freopen("A_in.out","w",stdout);
	scanf("%d",&t);
	while(t--)
	{
		
		scanf("%lf%lf%lf%lf%lf",&X,&S,&R,&T,&N);
		sum=0;
		for(i=0;i<N;i++)
		{
			ans[i].input();
		}
		printf("Case #%d: ",++Case);
		sort(ans,ans+(int)N,cmp);
		slove();
	}
	return 0;
}