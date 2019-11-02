#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <set>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
#include <map>
using namespace std;
#define INF 0x73737373
#define EPS 1e-8
#define lson l, m, rt<<1
#define rson m+1, r, rt<<1|1
typedef struct QT
{
    int x, y,len;
    string step;
    QT(){}
    QT(int xx, int yy, int ll){x = xx, y = yy, len = ll, step.clear();}
}QueueTemp;
const int dirc[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
char dircc[] = "NESW";
map <pair<int, int>, bool> hash;
void bfs(int sx, int sy, int ex, int ey, int ca)
{
    queue<QueueTemp> q;
    q.push(QT(sx, sy, 1));
    while(!q.empty())
    {
        QueueTemp now = q.front();
        if(now.x == ex && now.y == ey){
            printf("Case #%d: ", ca);
            cout << now.step << endl;
            return;
        }
        q.pop();
        for(int i = 0; i < 4; i++){
            int nx = now.x + dirc[i][0] * now.len;
            int ny = now.y + dirc[i][1] * now.len;
            if(!hash[make_pair(nx, ny)]){
                hash[make_pair(nx, ny)] = true;
                QueueTemp next = QT(nx, ny, now.len + 1);
                next.step = now.step + dircc[i];
                q.push(next);
            }
        }
    }
}
int main()
{
    int t;
    freopen("B-small-attempt0.in", "r", stdin);
    freopen("b.out", "w", stdout);
    cin >> t;
    for(int ca = 1; ca <= t; ca++)
    {
        int x, y;
        hash.clear();
        cin >> x >> y;
        bfs(0, 0, x, y, ca);
    }
	return 0;
}
