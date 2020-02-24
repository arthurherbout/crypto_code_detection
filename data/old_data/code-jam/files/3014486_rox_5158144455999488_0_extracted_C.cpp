#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>

using namespace std;

int w,h,b;
int g[555][111];
const int MAX_V = 2222222;
struct edge { int to,cap,rev; };
vector<edge> G[MAX_V];
int level[MAX_V];
int iter[MAX_V];

void add_edge(int f,int t,int cap)
{
  G[f].push_back((edge){t,cap,(int)G[t].size()});
  G[t].push_back((edge){f,0,(int)G[f].size()-1});
}

void bfs(int s)
{
  memset(level,-1,sizeof(level));
  queue<int> q;
  level[s] = 0;
  q.push(s);
  while( !q.empty() ) {
    int v = q.front(); q.pop();
    for( int i = 0; i < int(G[v].size()); i++ ) {
      edge& e = G[v][i];
      if( e.cap > 0 && level[e.to] < 0 ) {
        level[e.to] = level[v]+1;
        q.push(e.to);
      }
    }
  }
}

const int inf = 1<<21;

int dfs(int v,int t,int f)
{
  if( v == t ) return f;
  for( int &i = iter[v]; i < int(G[v].size()); i++ ) {
    edge& e = G[v][i];
    if( e.cap > 0 && level[v] < level[e.to] ) {
      int d  = dfs(e.to,t,min(f,e.cap));
      if( d > 0 ) {
        e.cap -= d;
        G[e.to][e.rev].cap += d;
        return d;
      }
    }
  }
  return 0;
}

int max_flow(int s,int t)
{
  int flow = 0;
  for(;;) {
    bfs(s);
    if( level[t] < 0 ) return flow;
    memset(iter,0,sizeof(iter));
    int f;
    while( (f = dfs(s,t,inf)) > 0 ) {
      flow += f;
    }
  }
}

int main(void)
{
  int T;
  scanf("%d",&T);
  for( int T_ = 1; T_ <= T; T_++ ) {
    printf("Case #%d: ",T_);
    scanf("%d%d%d",&w,&h,&b);
    for( int i = 0; i < h; i++ ) {
      for( int j = 0; j < w; j++ ) {
        g[i][j] = 0;
      }
    }
    for( int i = 0; i < b; i++ ) {
      int x0,y0,x1,y1; scanf("%d%d%d%d",&x0,&y0,&x1,&y1);
      g[y0][x0] += 1;
      g[y1+1][x1+1] += 1;
      g[y1+1][x0] -= 1;
      g[y0][x1+1] -= 1;
    }
    for( int i = 0; i < h; i++ ) {
      for( int j = 0; j < w-1; j++ ) {
        g[i][j+1] += g[i][j];
      }
    }
    for( int i = 0; i < h-1; i++ ) {
      for( int j = 0; j < w; j++ ) {
        g[i+1][j] += g[i][j];
      }
    }
    /*
    puts("");
    for( int i = 0; i < h; i++ ) {
      for( int j = 0; j < w; j++ ) {
        printf("%d",g[i][j]);
      }
      puts("");
    }
    //*/
    int res = w+1;
    int t = w*h;
    for( int i = 0; i <= t*2+1; i++ ) G[i].clear();
    for( int i = 0; i < h; i++ ) {
      for( int j = 0; j < w; j++ ) {
        add_edge(i*w+j+1,i*w+j+1+t,1);
      }
    }
    for( int i = 0; i < h; i++ ) {
      for( int j = 0; j < w; j++ ) {
        int dx[]={0,1,-1,0};
        int dy[]={1,0,0,-1};
        for( int k = 0; k < 4; k++ ) {
          if( j+dx[k] < 0 || j+dx[k] >= w ) continue;
          if( i+dy[k] < 0 || i+dy[k] >= h ) continue;
          if( !g[i][j] && !g[i+dy[k]][j+dx[k]] ) {
            add_edge(i*w+j+1+t,(i+dy[k])*w+(j+dx[k])+1,1);
          }
        }
      }
    }
    for( int i = 0; i < w; i++ ) {
      if( !g[0][i] ) add_edge(0,i+1,1);
    }
    for( int i = 0; i < w; i++ ) {
      if( !g[h-1][i] ) add_edge(i+1+t+t-w,t*2+1,1);
    }
    printf("%d\n",max_flow(0,t*2+1));
  }
  return 0;
}
