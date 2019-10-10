#include <iostream>
#include <cstdio>
#include <string.h>
#include <queue>
#include <map>
#include <string>
using namespace std;

int sx, sy;
struct Node
{
    int x, y;
    Node(){};
    Node(int a, int b)
    {
        x = a, y = b;
    }
};
struct NN
{
    Node cnt;
    int step;
    NN(){};
    NN(Node a, int b)
    {
        cnt = a;
        step = b;
    }
};
bool operator < (Node a, Node b)
{
    if (a.x != b.x)
        return a.x < b.x;
    return a.y < b.y;
}
map<Node, Node> mp;
int dx[4] = {-1, 0, 0, 1};
int dy[4] = {0, -1, 1, 0};

void bfs()
{
    mp.clear();
    mp[Node(0, 0)] = Node(0, 0);
    queue<NN> q;
    q.push(NN(Node(0, 0), 1));
    NN t;
    int tx, ty;
    while(!q.empty())
    {
        t = q.front();
        q.pop();
        if (t.cnt.x == sx && t.cnt.y == sy)
            break;
        for (int i = 0; i < 4; i++)
        {
            tx = t.cnt.x + dx[i] * t.step;
            ty = t.cnt.y + dy[i] * t.step;
            if (mp.find(Node(tx, ty)) == mp.end())
            {
                mp[Node(tx, ty)] = t.cnt;
                q.push(NN(Node(tx, ty), t.step + 1));
            }
        }
    }
}
char judge(int cntx, int cnty, int nxtx, int nxty)
{
    if (cntx == nxtx)
    {
        if (cnty < nxty)
            return 'N';
        return 'S';
    }
    if (cntx < nxtx)
        return 'E';
    else
        return 'W';
}
int main()
{
    freopen("t.in", "r", stdin);
    freopen("ans.txt", "w", stdout);
    int t;
    scanf("%d", &t);
    for (int z = 1; z <= t; z++)
    {
        scanf("%d%d", &sx, &sy);
        string ans = "";
        bfs();
        int cntx = sx, cnty = sy;
        map<Node, Node>::iterator p;
        while (cntx != 0 || cnty != 0)
        {
            p = mp.find(Node(cntx, cnty));
            ans = judge((p->second).x, (p->second).y, cntx, cnty) + ans;
            cntx = (p->second).x;
            cnty = (p->second).y;
        }
        printf("Case #%d: ", z);
        cout << ans << endl;
    }
    return 0;
}
