#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <deque>

using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef unsigned long ul;
typedef unsigned short us;
typedef unsigned char uc;

int dist[500];
int deg[500];
bool A[500][500];

int p,w;

int dij()
{
  bool vis[500];
  memset(vis,0,sizeof(vis));

  deque<int> Q,Q2;
  Q.push_back(0); vis[0]=1;
  int d=1;

  while(!Q.empty())
  { Q2.clear();
    while(!Q.empty())
    {
      int i=Q.front(); Q.pop_front();
      for(int j=1;j<p;j++) if(A[i][j] && !vis[j]) { { vis[j]=1; Q2.push_back(j); dist[j]=d; } }
    }
      Q = Q2; d++;
  }
}

main()
{
  int cases;
  cin >> cases;
  for(int loop=1; loop<=cases; loop++)
  {
    cin >> p >> w;
    memset(A,0,sizeof(A));
    memset(deg,0,sizeof(deg));
    for(int i=0;i<w;i++) { int a,b; scanf("%d,%d",&a,&b); A[a][b]=A[b][a]=1; deg[a]++; deg[b]++; }
    dij();

    printf("Case #%d: ",loop);
    printf("%d ",dist[1]-1);

    int d=dist[1]-1, pos=1;
    bool used[500]; memset(used,0,sizeof(used));

    while(pos)
    {
      int M=0, best=-1;
      for(int i=0;i<p;i++) if(A[pos][i] & dist[i]==d && deg[i]>M) { best=i; M=deg[i]; }
      pos = best; used[pos]=1;
      d--;
    }

    int thr=0;
    for(int i=0;i<p;i++)
    {
      if (used[i]) continue;
      for(int j=0;j<p;j++) if(A[i][j] && used[j]) { thr++; break; }
    }
    printf("%d\n",thr);

    // puts("");
  }
}
