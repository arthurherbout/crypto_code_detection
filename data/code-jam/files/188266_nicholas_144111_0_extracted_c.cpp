
#include<iostream>
#include<cstring>
#include<vector>
#include<set>
#include<map>
using namespace std;
typedef __int64 LL;
LL time[32][32][4];
int N, M;
LL S[32][32], W[32][32], T0[32][32];

#define INF 100000000
#define QSIZE 200000
struct Node{
       int y, x, c;
       LL t;
       Node(int a=0, int b=0, int z=0, LL u=0){
           y = a;
           x = b;
           c = z;
           t = u;
       }
}Q[QSIZE];
bool IN(Node a){
     if(a.x >=1 || a.x <= M || a.y >= 1 || a.y <= N) return true;
     return false;
}
int front, rear;
int To[4][4][3] = {{ {0, 0, 2},{0, 1, 1},{1, 0, 2},{0, 0, 1} },{{0, 0, 3},{0, 0, 0},{1, 0, 3},{0, -1, 0} },{{-1, 0, 0},{0, 1, 3},{0, 0, 0},{0, 0, 3}},{{-1, 0, 1},{0, 0, 2},{0, 0, 1},{0, -1, 2}}};
LL Wait(int y, int x, LL t, int d){
    bool s;
    if(d == 0 || d == 2) s = 1;
    else s = 0;
    LL dt;
    LL kt;
    if(s){
        dt = t - T0[y][x];
        dt %= (S[y][x] + W[y][x]);
        if(dt < S[y][x]){
              kt = 0;
        }
        else
			kt = S[y][x] + W[y][x] - dt;
    }
    else{
        dt = t - T0[y][x];
        dt %= (S[y][x] + W[y][x]);
        
        if(dt >= S[y][x]) kt = 0;
        else  kt = S[y][x] - dt;
    }
    return t + kt;
}
bool Next(Node c, int d, Node& ne){
    ne.y = c.y + To[c.c][d][0];
    ne.x = c.x + To[c.c][d][1];
    ne.c = To[c.c][d][2];
    if(ne.y < 1 || ne.y > N || ne.x < 1 || ne.x > M) return 0;
    if(!(ne.y == c.y && ne.x == c.x)){
            ne.t = c.t + 2;
            return 1;
    }
    
    ne.t = Wait(ne.y, ne.x, c.t, d);
    ne.t += 1;
    return 1;
}
LL Solve(){
    memset(time, 63, sizeof(time));
    front = rear = 0;
    Q[rear++] = Node(N, 1, 1, 0);
    time[N][1][1] = 0;
    Node cur;
    Node ne;
    int d;
    while(front != rear){
        cur = Q[front++];
        front %= QSIZE;
        
        for(d = 0; d < 4; ++d){
              if(Next(cur, d, ne)){
                  if(ne.t < time[ne.y][ne.x][ne.c]){
                          time[ne.y][ne.x][ne.c] = ne.t;
                          Q[rear++] = ne;
                          rear %= QSIZE;
                  }
              }
              
        }
    }
    return time[1][M][2];
}
int main(){
   int casenum, TT, j, i;
   freopen("in.txt", "r", stdin);
   freopen("out.txt", "w", stdout);
    scanf("%d", &TT);
    for(casenum = 1; casenum <= TT; ++casenum){
       scanf("%d%d", &N, &M);  
       for(i = 1; i <= N; ++i){
           for(j =1; j <= M; ++j){
                 scanf("%I64d%I64d%I64d", &S[i][j], &W[i][j], &T0[i][j]);
                 T0[i][j] -= (S[i][j] + W[i][j]) * INF;
           }
       }  
	   LL ans = Solve();
       printf("Case #%d: %I64d\n", casenum, ans);
    }
}
