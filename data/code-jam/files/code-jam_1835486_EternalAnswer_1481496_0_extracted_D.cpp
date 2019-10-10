#include <cstdio>
#include <algorithm>
#include <cstring>
#define Sort sort

using namespace std;

const int Maxn=10000;
const int Maxm=2000000;
const int inf=99999999;

struct Tnode
{
	int x,y;
};

int can[30];
int pos[Maxn];
int date[Maxm],next[Maxm],map[Maxm],other[Maxm];
int tmpmap[Maxm];
int q[Maxn];
int last[Maxn],dist[Maxn],gap[Maxn],flow[Maxn],path[Maxn];
Tnode a[Maxn];
int n,m,Test,tot,s,t,S,T;
char st[Maxn];

void add(int x,int y)
{	
	++tot;
	a[tot].x=x;a[tot].y=y;
}

bool Cmp(Tnode A,Tnode B)
{
	return A.x<B.x || (A.x==B.x && A.y<B.y);
}

void add1(int x,int y,int z)
{
	++tot;
	date[tot]=y;next[tot]=pos[x];pos[x]=tot;
	map[tot]=z;other[tot]=tot+1;
	++tot;
	date[tot]=x;next[tot]=pos[y];pos[y]=tot;
	map[tot]=0;other[tot]=tot-1;
}

void Add(int x,int y,int d,int u)
{
	if (!d) add1(x,y,u);
	else
	{
		add1(x,T,1);
		add1(S,y,1);
	}
}

void Bfs()
{
	memset(dist,-1,sizeof(dist));
	memset(gap,0,sizeof(gap));
	int l1=1,l2=1;
	dist[T]=0;
	q[1]=T;
	for (;l1<=l2;++l1)
	{
		int now=q[l1];
		++gap[dist[now]];
		for (int k=pos[now];k;k=next[k])
			if (map[other[k]] && dist[date[k]]==-1)
			{
				dist[date[k]]=dist[now]+1;
				q[++l2]=date[k];
			}
	}
}

void Sap()
{
	Bfs();
	for (int i=S;i<=T;++i) last[i]=pos[i];
	int now=S;
	flow[S]=inf;
	int res=0;
	for (;dist[S]<T+1;)
	{
		bool flag=false;
		for (int k=last[now];k;k=next[k])
		{
			if (!map[k] || dist[date[k]]+1!=dist[now]) continue;
			int nex=date[k];
			flow[nex]=min(flow[now],map[k]);
			flag=true;
			path[nex]=last[now]=k;
			now=nex;
			if (now!=T) break;
			for (int x=T;x!=S;x=date[other[path[x]]])
			{
				map[path[x]]-=flow[T];
				map[other[path[x]]]+=flow[T];
			}
			now=S;
			break;
		}
		if (flag) continue;
		int tmp=T+2;
		for (int k=pos[now];k;k=next[k])
			if (map[k] && dist[date[k]]+1<tmp)
			{
				tmp=dist[date[k]]+1;
				last[now]=k;
			}
		--gap[dist[now]];
		if (!gap[dist[now]]) break;
		dist[now]=tmp;
		++gap[dist[now]];
		if (now!=S) now=date[other[path[now]]];
	}
}

bool Check(int mid)
{
	tmpmap[tot-1]=mid;
	for (int i=1;i<=tot;++i) map[i]=tmpmap[i];
	Sap();
	for (int k=pos[S];k;k=next[k])
		if (map[k]) 
			return false;
	return true;
}

int Min_flow()
{
	add1(t,s,0);
	for (int i=1;i<=tot;++i) tmpmap[i]=map[i];
	int l=1,r=n;
	for (;l<r;)
	{
		int mid=(l+r)/2;
		if (Check(mid)) r=mid;
		else l=mid+1;
	}
	return l;
}
	
int main()
{
	freopen("x.in","r",stdin);
	freopen("x.out","w",stdout);
	
	memset(can,-1,sizeof(can));
	can['o'-'a']=0;can['i'-'a']=1;can['e'-'a']=3;
	can['a'-'a']=4;can['s'-'a']=5;can['t'-'a']=7;
	can['b'-'a']=8;can['g'-'a']=9;
	scanf("%d",&Test);
	for (int ii=1;ii<=Test;++ii)
	{
		scanf("%d",&m);
		scanf("%s",st);
		n=strlen(st);
		tot=0;
		for (int i=0;i<n-1;++i)
		{
			add(st[i]-'a',st[i+1]-'a');
			int u=tot;
			if (can[a[u].x]!=-1) add(can[a[u].x]+26,a[u].y);
			if (can[a[u].y]!=-1) add(a[u].x,can[a[u].y]+26);
			if (can[a[u].x]!=-1 && can[a[u].y]!=-1)
				add(can[a[u].x]+26,can[a[u].y]+26);
		}
		Sort(a+1,a+tot+1,Cmp);
		int tmp=tot;
		tot=1;
		for (int i=2;i<=tmp;++i)
			if (a[i].x!=a[i-1].x || a[i].y!=a[i-1].y)
				a[++tot]=a[i];
		n=tot;
		tot=0;
		memset(pos,0,sizeof(pos));
		s=n*2+1;t=s+1;S=0;T=t+1;
		for (int i=1;i<=n;++i)
		{
			Add(s,i,0,1);
			Add(i+n,t,0,1);
			Add(i,i+n,1,1);
			for (int j=1;j<=n;++j)
				if (i!=j && a[i].y==a[j].x)
					Add(i+n,j,0,1);
		}
		printf("Case #%d: ",ii);
		printf("%d\n",n+Min_flow());
	}
	
	return 0;
}