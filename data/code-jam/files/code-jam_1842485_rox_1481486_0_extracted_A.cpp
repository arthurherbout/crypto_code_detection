#include <cstdio>
#include <queue>
#include <algorithm>

using namespace std;

int d[11111],l[11111];
bool used[11111];

struct state {
  int n;
  int v;
  state(){}
  state(int n,int v):n(n),v(v){}
};

int main(void)
{
  int t; scanf("%d",&t);
  for(int T = 1; T <= t; T++){
    printf("Case #%d: ",T);
    int n; scanf("%d",&n);
    for(int i = 0; i < n; i++){
      scanf("%d%d",d+i,l+i);
    }
    int D; scanf("%d",&D);

    queue<state> q;
    q.push(state(0,d[0]));

    fill(used,used+11111,false);
    used[0] = true;

    while( !q.empty() ) {
      state p = q.front(); q.pop();
      //printf("%d %d\n",p.n,p.v);
      
      if( d[p.n]+p.v >= D ) {
        puts("YES");
        goto NEXT;
      }
      
      for(int i = p.n; i < n; i++){
        if( d[p.n]+p.v < d[i] ) {
          break;
        }
        if( !used[i] ) {
          used[i] = true;
          q.push(state(i,d[i] - d[p.n] - (max(0,d[i] - d[p.n] - l[i]))));
        }
      }
      
    }
    puts("NO");
  NEXT:;
    
  }
}
