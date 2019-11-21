#include<stdio.h>
#include<memory.h>
#include<cmath>



const double PI = acos(-1.0);
const int MAXINT = 0x7FFFFFFF;
typedef long long int64;
const int64 MAXINT64 = 0x7FFFFFFFFFFFFFFFLL;
const int SIZE = 262143;
#define PS(x) (cout<<#x<<": "<<endl)
#define DB(x) (cout<<#x<<": "<<x<<endl)
#define MST(t,v) memset(t,v,sizeof(t))
#define SHOW1(a,n) (PS(a),_show1(a,n))
#define SHOW2(a,r,c) (PS(a),_show2(a,r,c))

template<class T>void _show1(T a, int n){for(int i=0; i<n; ++i) cout<<a[i]<<' '; cout<<endl;}
template<class T>void _show2(T a, int r, int l){for(int i=0; i<r; ++i)_show1(a[i],l);cout<<endl;}
template<class T> inline void checkmax(T &a,T b){if(b>a) a=b;} 
template<class T> inline void checkmin(T &a,T b){if(b<a) a=b;}

long long one = 1;
struct Node
{
    int cx, cy;
    long long vis[4];
};
int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};
char G[20][20];
Node que[SIZE + 1];
int R, C, front, rear, step;
int nic[20][20];


bool inside(int y, int x)
{
     return y >= 0 && y < R && x >= 0 && x < C;
}
void check(Node& ne)
{
     int y2, x2, i, k;
     for(i = 0; i < 4; ++i)
     {
         y2 = ne.cy;
         x2 = ne.cx;
         while(inside(y2 + dy[i], x2 + dx[i]) && !( G[y2 + dy[i]][x2 + dx[i]] == '#'))
             y2 += dy[i],x2 += dx[i];
      
         k = (y2 * C + x2);
         ne.vis[k / 60] |= one << (k % 60);
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
            if(G[i][j] == 'O') ii = i, jj = j;
    
	
	
	
	cur.cy = ii; cur.cx = jj; 
    memset(cur.vis, 0, sizeof(cur.vis));
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
        front &= SIZE;
     
        for(i = 0; i < 4; ++i)
        {
            ne.cy = cur.cy + dy[i];
            ne.cx = cur.cx + dx[i];
            if(!inside(ne.cy, ne.cx) || G[ne.cy][ne.cx] == '#')
            {
                for(k = 0; k < R * C; ++k)
                   if(cur.vis[k / 60] & (one << (k % 60)))
                   {
                   
                       ne.cy = k / C;
                       ne.cx = k % C;
                    
                       memset(ne.vis, 0, sizeof(ne.vis));
                       if(G[ne.cy][ne.cx] == 'X') return step;
                       if(nic[ne.cy][ne.cx] <= step) continue;
                       nic[ne.cy][ne.cx] = step;
                       check(ne);
                       que[rear++] = ne;
                       rear &= SIZE;
                   }
                continue;
            }
            memcpy(ne.vis, cur.vis, sizeof(long long) * 4);
            check(ne);
            if(G[ne.cy][ne.cx] == 'X') return step;
                 if(nic[ne.cy][ne.cx] <= step) continue;
           nic[ne.cy][ne.cx] = step;
           que[rear++] = ne;
          
		   rear &= SIZE;
        }
     }
        
    }
    return -1;
}
int main()
{
    int T, i, j;
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
    scanf("%d", &T);
    for (int ctr = 1; ctr <= T; ctr++)
    {
        scanf("%d%d", &R, &C);
        for(i = 0; i < R; ++i)
                scanf("%s", G[i]);
        int ret = BFS();
        printf("Case #%d: ", ctr);
        if(ret == -1)
            printf("THE CAKE IS A LIE\n");
        else printf("%d\n", ret);
    }
}
