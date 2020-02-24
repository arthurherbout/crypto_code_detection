#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <cstring>

using namespace std;

typedef long long LL;
typedef unsigned long long ULL;

#define SIZE(x) (int((x).size()))
#define rep(i,l,r) for (int i=(l); i<=(r); i++)
#define repd(i,r,l) for (int i=(r); i>=(l); i--)
#define rept(i,c) for (typeof((c).begin()) i=(c).begin(); i!=(c).end(); i++)

#ifndef ONLINE_JUDGE
#define debug(x) { cerr<<#x<<" = "<<(x)<<endl; }
#else
#define debug(x) {}
#endif

#define maxn 2020

char ch[maxn];
int ins[maxn], id[maxn], used[maxn], last[maxn];

struct edge
{
	int id, wg, anti;
	edge() {}
	edge(int id, int wg, int anti): id(id), wg(wg), anti(anti) {}
};

vector<edge> e[maxn];

void add(int x, int y, int z)
{
	e[x].push_back(edge(y,z,e[y].size()));
	e[y].push_back(edge(x,0,e[x].size()-1));
}

int source, sink, flow, augc, found;
int h[maxn], vh[maxn], cur[maxn];

void sap(int m)
{
	if (m==sink)
	{
		found=1; flow+=augc;
		return;
	}
	
	int augc2=augc;
	vector<edge>::iterator it=e[m].begin()+cur[m];
	while (it<e[m].end())
	{
		if (it->wg && h[m]==h[it->id]+1)
		{
			cur[m]=it-e[m].begin();
			augc=min(augc,it->wg);
			sap(it->id);
			if (found) break;
			if (h[source]>=sink) return;
			augc=augc2;
		}
		it++;
	}
	
	if (found)
	{
		it->wg-=augc;
		e[it->id][it->anti].wg+=augc;
	}
	else
	{
		int minh=sink-1, minhi=0;
		rept(it,e[m])
			if (it->wg && h[it->id]<minh)
			{
				minh=h[it->id]; minhi=it-e[m].begin();
			}
		vh[h[m]]--; if (vh[h[m]]==0) h[source]=sink;
		h[m]=minh+1;
		vh[h[m]]++;
		cur[m]=minhi;
	}
}

void networkflow()
{
	flow=0; memset(h,0,sizeof h); 
	memset(vh,0,sizeof vh); vh[0]=sink;
	memset(cur,0,sizeof cur);
	while (h[source]<sink)
	{
		augc=0x7fffffff; found=0;
		sap(source);
	}
}

int check(int n0, int ne)
{
	source=ne+1; sink=ne+2;
	rep(i,n0,sink) e[i].clear();
	rep(i,1,2000) last[i]=n0;
	memset(used,0,sizeof used);
	memset(ins,0,sizeof ins);
	rep(i,n0,ne)
		if (ch[i]=='E')
		{
			if (id[i]>0)
			{
				if (ins[id[i]])
				{
					used[last[id[i]]]=1;
					rep(k,last[id[i]],i)
						if (ch[k]=='L' && id[k]==0)
						{
							add(last[id[i]],k,1);
						}
				}
				ins[id[i]]=1;
				last[id[i]]=i;
			}
		}
		else  
		{
			if (id[i]>0)
			{
				if (!ins[id[i]])
				{
					rep(k,last[id[i]],i)
						if (ch[k]=='E' && id[k]==0)
						{
							add(k,i,1);
						}
				}
				else  
				{
					rep(k,last[id[i]],i)
						if (ch[k]=='E' && id[k]==0)
						{
							add(k,i,1);
						}
					add(last[id[i]],i,1);
					used[last[id[i]]]=1;
				}
				ins[id[i]]=0;
				last[id[i]]=i;
				used[i]=1;
			}
			else
			{
				rep(k,n0,i)
					if (ch[k]=='E')
					{
						add(k,i,1);
					}
			}
		}
	
	int cz=0;
	rep(i,n0,ne)
		if (ch[i]=='E')
			add(source,i,1);
		else  
		{
			add(i,sink,1);
			cz++;
		}
	
	networkflow();
	return cz==flow;
}

void lemon()
{
	int n; scanf("%d",&n);
	rep(i,1,1010) ch[i]='E', id[i]=0;
	rep(i,1,n) 
	{
		char cc; 
		while (1)
		{
			scanf("%c",&cc); if (cc=='E' || cc=='L') break;
		}
		ch[1010+i]=cc;
		scanf("%d",&id[1010+i]);
	}
	
	int d=0, mind=0;
	rep(i,1011,1010+n) 
	{
		if (ch[i]=='E') d++; else d--;
		mind=min(mind,d);
	}
	
	rep(i,0,n+5) 
		if (check(1011-i,1010+n))
		{
			printf("%d\n",i+d);
			return;
		}
	printf("CRIME TIME\n");
}

int main()
{
	ios::sync_with_stdio(true);
	#ifndef ONLINE_JUDGE
		freopen("C.in","r",stdin);
	#endif
	int tcase; scanf("%d",&tcase);
	rep(nowcase,1,tcase) 
	{
		printf("Case #%d: ",nowcase);
		lemon();
	}
	return 0;
}

