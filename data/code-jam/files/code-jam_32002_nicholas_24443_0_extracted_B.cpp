#include<stdio.h>
#include<memory.h>
#include <cmath>
long long one = 1;
const double PI = acos(-1.0);
const int MAXINT = 0x7FFFFFFF;
const int QSIZE = 131071;
typedef long long int64;
const int64 MAXINT64 = 0x7FFFFFFFFFFFFFFFLL;

#define PS(x) (cout<<#x<<": "<<endl)
#define DB(x) (cout<<#x<<": "<<x<<endl)
#define MST(t,v) memset(t,v,sizeof(t))
#define SHOW1(a,n) (PS(a),_show1(a,n))
#define SHOW2(a,r,c) (PS(a),_show2(a,r,c))

template<class T>void _show1(T a, int n){for(int i=0; i<n; ++i) cout<<a[i]<<' '; cout<<endl;}
template<class T>void _show2(T a, int r, int l){for(int i=0; i<r; ++i)_show1(a[i],l);cout<<endl;}
template<class T> inline void checkmax(T &a,T b){if(b>a) a=b;} 
template<class T> inline void checkmin(T &a,T b){if(b<a) a=b;}

struct Node
{
    int cx, cy;
    long long vis;
};
int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};

char map[20][20];
Node que[QSIZE + 1];
int R, C, front, rear, step;
int nic[20][20];

bool check2(int y, int x)
{
     return y >= 0 && y < R && x >= 0 && x < C;
}
void check(Node& ne)
{
     int y2, x2, i;
     for(i = 0; i < 4; ++i)
     {
         y2 = ne.cy;
         x2 = ne.cx;
         while(check2(y2 + dy[i], x2 + dx[i]) && !( map[y2 + dy[i]][x2 + dx[i]] == '#'))
             y2 += dy[i],x2 += dx[i];
      
         ne.vis |= one << (y2 * C + x2);
     }
}
int BFS()
{
    memset(nic, 63, sizeof(nic));
    int i, j, re, k, ii, jj;
    Node cur, ne;
    front = rear = step = 0;
    for(i = 0; i < R; ++i)
        for(j = 0; j < C; ++j)
            if(map[i][j] == 'O') ii = i, jj = j;
    cur.cy = ii; cur.cx = jj; 
    cur.vis = 0;
    check(cur);
    
    nic[i][j] = 0;
    que[rear++] = cur;
    while(front != rear)
    {
       re = rear;
       ++step;
      while(front != re)
      {
        cur = que[front++];
        front &= QSIZE;
        for(i = 0; i < 4; ++i)
        {
            ne.cy = cur.cy + dy[i];
            ne.cx = cur.cx + dx[i];
            if(!check2(ne.cy, ne.cx) || map[ne.cy][ne.cx] == '#')
            {
                for(k = 0; k < R * C; ++k)
                   if(cur.vis & (one << k))
                   {
                       ne.cy = k / C;
                       ne.cx = k % C;
                       ne.vis = 0;
                       if(map[ne.cy][ne.cx] == 'X') return step;
                       if(nic[ne.cy][ne.cx] <= step) continue;
                       nic[ne.cy][ne.cx] = step;
                       check(ne);
                       que[rear++] = ne;
                       rear &= QSIZE;
                   }
                continue;
            }
            ne.vis = cur.vis;
            check(ne);
            if(map[ne.cy][ne.cx] == 'X') return step;
                 if(nic[ne.cy][ne.cx] <= step) continue;
           nic[ne.cy][ne.cx] = step;
           que[rear++] = ne;
           rear &= QSIZE;
        }
     }
        
    }
    return -1;
}
int main()
{
    int T, res, i, j;
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
    scanf("%d", &T);
    for (int ctr = 1; ctr <= T; ctr++)
    {
        scanf("%d%d", &R, &C);
        for(i = 0; i < R; ++i)
              scanf("%s", map[i]);
        res = BFS();
        printf("Case #%d: ", ++ctr);
        if(res == -1)
            printf("THE CAKE IS A LIE\n");
        else printf("%d\n", res);
    }
}
