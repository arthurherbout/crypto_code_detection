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

#define maxn 5010
#define INF 1000000000

int dist[maxn];
vector< pair<int,int> > e[maxn];
int n;

void add(int x, int y, int z)
{
	if (z<0) z=0;
	e[x].push_back(make_pair(y,z));
}

int delx[4]={0,0,1,1};
int dely[4]={0,1,1,0};

void dijkstra(int source)
{
	rep(i,1,n) dist[i]=INF; dist[source]=0;
	set< pair<int,int> > s;
	s.insert(make_pair(0,source));
	while (!s.empty())
	{
		int x=s.begin()->second; s.erase(s.begin());
		rept(it,e[x]) 
			if (dist[x]+it->second<dist[it->first])
			{
				set< pair<int,int> >::iterator it2=s.find(make_pair(dist[it->first],it->first));
				if (it2!=s.end()) s.erase(it2);
				dist[it->first]=dist[x]+it->second;
				s.insert(make_pair(dist[it->first],it->first));
			}
	}
}

int overlap(int al, int ar, int bl, int br)
{
	if (al>bl) swap(al,bl), swap(ar,br);
	return ar>=bl;
}

int getdis(int al, int ar, int bl, int br)
{
	if (al>bl) swap(al,bl), swap(ar,br);
	return bl-ar;
}

int X0[maxn], Y0[maxn], X1[maxn], Y1[maxn];

void lemon()
{
	int w,h,m; scanf("%d%d%d",&w,&h,&m);
	rep(i,1,m) 
	{
		scanf("%d%d%d%d",&X0[i],&Y0[i],&X1[i],&Y1[i]);
		X1[i]++; Y1[i]++;
	}
	
	n=m+2;
	rep(i,1,n) e[i].clear();
	
	add(m+1,m+2,w);
	rep(i,1,m) 
	{
		add(m+1,i,X0[i]);
		add(i,m+2,w-X1[i]);
	}
	
	rep(i,1,m)
		rep(j,1,m)
			if (i!=j)
			{
				if (overlap(X0[i],X1[i],X0[j],X1[j]))
				{
					add(i,j,getdis(Y0[i],Y1[i],Y0[j],Y1[j]));
					continue;
				}
				if (overlap(Y0[i],Y1[i],Y0[j],Y1[j]))
				{
					add(i,j,getdis(X0[i],X1[i],X0[j],X1[j]));
					continue;
				}
				add(i,j,max(getdis(X0[i],X1[i],X0[j],X1[j]),getdis(Y0[i],Y1[i],Y0[j],Y1[j])));
			}
	
	dijkstra(m+1);
	
	printf("%d\n",dist[m+2]);
	
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

