#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#define Sort sort

using namespace std;

const int Maxn=10;
const int Maxm=100;
const double wc=1e-7;

bool f[100][Maxm];
int v[Maxn],d[Maxn],dir[Maxn];
double dis[Maxn];
double a[100];
int n,m,tot,Test;
int u;
char ch;
double len;

int Calc(double t)
{
	int res=0;
	for (int i=0;i<n;++i)
		dis[i]=t*v[i]+d[i];
	for (int i=0;i<n;++i)
	{
		bool flag=true;
		for (int j=0;j<n;++j)
			if (i!=j)
			{
				if (fabs(dis[i]-dis[j])<len-wc) flag=false;
				if (fabs(dis[i]+len-dis[j])<wc && v[i]>v[j]) flag=false;
				if (fabs(dis[i]-dis[j]-len)<wc && v[i]<v[j]) flag=false;
			}
		if (flag) res|=1 << i;
	}
	return res;
}

bool Check(double t,int sub)
{
	for (int i=0;i<n;++i)
		dis[i]=t*v[i]+d[i];
	bool flag=true;
	for (int i=0;flag && i<n;++i)
		for (int j=i+1;flag && j<n;++j)
		{
			int t1=((sub & (1 << i))!=0);
			int t2=((sub & (1 << j))!=0);
			if (t1==t2)
			{
				if (fabs(dis[i]-dis[j])<len-wc) flag=false;
				if (fabs(dis[i]+len-dis[j])<wc && v[i]>v[j]) flag=false;
				if (fabs(dis[i]-dis[j]-len)<wc && v[i]<v[j]) flag=false;
			}
		}
	return flag;
}

int main()
{
	freopen("x.in","r",stdin);
	freopen("x.out","w",stdout);
	
	len=5;
	scanf("%d\n",&Test);
	for (int ii=1;ii<=Test;++ii)
	{
		scanf("%d\n",&n);
		u=0;
		for (int i=0;i<n;++i)
		{
			scanf("%c%d%d\n",&ch,&v[i],&d[i]);
			if (ch=='R') dir[i]=1,u+=1 << i;
			else dir[i]=0;
		}
		printf("Case #%d: ",ii);
		tot=0;
		bool flag=true;
		for (int i=0;i<n;++i)
			for (int j=i+1;j<n;++j)
				if (dir[i]==dir[j] && fabs(d[i]-d[j])<len-wc)
					flag=false;
		if (!flag)
		{
			printf("0\n");
			continue;
		}
		
		for (int i=0;i<n;++i)
			for (int j=0;j<n;++j)
				if (d[i]<d[j] || (d[i]==d[j] && i<j))
				{
					if (d[i]+5<=d[j] && v[i]>v[j])
						a[++tot]=(-len+d[j]-d[i])/((double)v[i]-v[j]);
					if (v[i]>v[j])
						a[++tot]=(len+d[j]-d[i])/((double)v[i]-v[j]);
				}
		
		if (!tot)
		{
			printf("Possible\n");
			continue;
		}
		
		Sort(a,a+tot);
		int tmp=tot;
		tot=1;
		for (int i=2;i<=tmp;++i)
			if (fabs(a[i]-a[i-1])>wc) a[++tot]=a[i];
		memset(f,false,sizeof(f));
		f[0][u]=true;
		a[tot+1]=a[tot]+(double)1000;
		for (int i=0;i<=tot;++i)
		{
			int can=Calc(a[i]);
			for (int j=0;j<1 << n;++j)
				if (f[i][j])
				{
					for (int k=0;k<1 << n;++k)
					{
						int sub=k ^ j;
						if ((sub & can)==sub)
							f[i+1][k]=true;
					}
				}
			for (int j=0;j<1 << n;++j)
				if (f[i+1][j])
					f[i+1][j]=Check(a[i],j);
		}
		
		int p=0;
		for (int i=0;i<=tot+1;++i)
		{
			bool flag=false;
			for (int j=0;j<1 << n;++j)
				flag|=f[i][j];
			if (flag) p=i;
		}
		
		if (p==tot+1) printf("Possible\n");
		else printf("%.7lf\n",a[p]);
	}
	
	return 0;
}