#include <cstdio>
#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

#define MAXN 405
#define MAXM 2005

class Queue
{
private:
	int Q[MAXN],head,tail;
public:
	void clear() { head=tail=0; }
	bool empty() { return head==tail; }
	void push(int v)
	{
		Q[tail++]=v;
		if(tail>=MAXN) tail-=MAXN;
	}
	int pop()
	{
		int ret=Q[head++];
		if(head>=MAXN) head-=MAXN;
		return ret;
	}
};

bool vst[MAXN];
int dis[MAXN];
int N,M;

typedef struct tNode
{
	int v;
	tNode *nxt;
}Edge;
Edge E1[MAXN],E2[MAXN],mem[MAXM*10];
int eCnt;

int res;

Queue Q;

typedef struct
{
	int u,v;
}edge;
edge e[MAXM];

int path[MAXN];

void init()
{
	for(int i=0;i<N;i++)
		E1[i].nxt=E2[i].nxt=NULL;
	eCnt=0;
}

void Add_Edge(Edge E[],int u,int v)
{
	Edge *p=&mem[eCnt++];
	p->v=v,p->nxt=E[u].nxt,E[u].nxt=p;
}

void bfs()
{
	for(int i=0;i<N;i++)
		vst[i]=0,dis[i]=10000;
	vst[0]=true,dis[0]=0;
	Q.clear();
	Q.push(0);
	while(!Q.empty())
	{
		int u=Q.pop();
		for(Edge *p=E1[u].nxt;p!=NULL;p=p->nxt)
		{
			if(vst[p->v]) continue;
			vst[p->v]=true;
			dis[p->v]=dis[u]+1;
			Q.push(p->v);
		}
	}
}

void dfs(int u,int depth)
{
	if(u==1)
	{
		memset(vst,0,sizeof(vst));
		for(int i=0;i<depth;i++)
			for(Edge *p=E1[path[i]].nxt;p!=NULL;p=p->nxt)
				vst[p->v]=true;
		for(int i=0;i<depth;i++)
			vst[path[i]]=false;
		int cnt=0;
		for(int i=1;i<N;i++) if(vst[i]) cnt++;
		res=max(res,cnt);
		return ;
	}
	path[depth]=u;
	for(Edge *p=E2[u].nxt;p!=NULL;p=p->nxt)
		dfs(p->v,depth+1);
}

int main()
{
	freopen("D-small-attempt0.in","r",stdin);
	freopen("lowesy_D.out","w",stdout);
	int _,cases=1;
	scanf("%d",&_);
	char str[1000];
	while(_--)
	{
		scanf("%d%d",&N,&M);
		init();
		int u,v;
		for(int i=0;i<M;i++)
		{
			scanf("%s",str);
			u=0,v=0;
			int L=strlen(str),pos=0;
			while(pos<L&&str[pos]>='0'&&str[pos]<='9')
				u=u*10+str[pos++]-'0';
			while(pos<L&&(str[pos]<'0'||str[pos]>'9'))
				pos++;
			while(pos<L&&str[pos]>='0'&&str[pos]<='9')
				v=v*10+str[pos++]-'0';
			e[i].u=u,e[i].v=v;
			Add_Edge(E1,u,v);
			Add_Edge(E1,v,u);
		}
		bfs();
		for(int i=0;i<M;i++)
		{
			if(dis[e[i].u]==dis[e[i].v]+1)
				Add_Edge(E2,e[i].v,e[i].u);
			if(dis[e[i].v]==dis[e[i].u]+1)
				Add_Edge(E2,e[i].u,e[i].v);
		}
		res=0;
		dfs(0,0);
		printf("Case #%d: %d %d\n",cases++,dis[1]-1,res);
	}
	return 0;
}