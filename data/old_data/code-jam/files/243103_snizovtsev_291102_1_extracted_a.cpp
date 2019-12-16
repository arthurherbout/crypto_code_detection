#include <climits>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <queue>
#include <algorithm>
#include <map>
using namespace std;

#define foreach(iter, cont) \
    for (typeof((cont).begin()) iter = (cont).begin(); iter != (cont).end(); iter++)
typedef long long LL;

const int maxs = 13;
int H, W;
int boxes_count;
class Boxes {
public:
    Boxes() {memset(data, 0, sizeof(data));}
    int data[maxs];

    bool stable(int i, int j)
    {
        bool up, down, left, right;
        up   = (i != 0) && (data[i-1] & (1<<j));
        down = (i != H-1) && (data[i+1] & (1<<j));
        left = (j != 0) && (data[i] & (1<<(j-1)));
        right = (j != W-1) && (data[i] & (1<<(j+1)));
        return up || down || right || left;
    }

    bool stable()
    {
        if (boxes_count == 1)
            return true;
        for (int i = 0; i < H; i++) {
            if (data[i] == 0) continue;
            for (int j = 0; j < W; j++) {
                if ((data[i] & (1<<j)) == 0)
                    continue;
                if (!stable(i, j)) return false;
            }
        }
        return true;
    }

    void operator = (const Boxes& a) {
        for (int i = 0; i < H; i++)
            data[i] = a.data[i];
    }
};

bool operator < (const Boxes &a, const Boxes &b)
{
    for (int i = 0; i < H; i++) {
        if (a.data[i] == b.data[i])
            continue;
        return a.data[i] < b.data[i];
    }
    return false;
}

int solve()
{
    scanf("%d %d", &H, &W);
    char board[H][W];
    int wall[H];
    memset(wall, 0, sizeof(wall));
    boxes_count = 0;

    Boxes initial;
    Boxes goal;

    for (int i = 0; i < H; i++)
    for (int j = 0; j < W; j++) {
        scanf("\t%c", &board[i][j]);
        if (board[i][j] == 'o' || board[i][j] == 'w')
            initial.data[i] |= 1 << j;
        if (board[i][j] == 'x' || board[i][j] == 'w')
            goal.data[i] |= 1 << j;
        wall[i] |= 1 << j;
        if (board[i][j] == 'o' || board[i][j] == 'w')
            boxes_count++;
    }

    queue <Boxes> que;
    map <Boxes, int> dist;

    dist[initial] = 0;
    que.push(initial);
    while (!que.empty()) {
        Boxes cur = que.front();
        Boxes next = cur;
        que.pop();
        bool stable = cur.stable();
        int turns = dist[cur];

        /*for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++)
                printf("%c", (cur.data[i] & (1<<j))?'1':'0');
            printf("\n");
        }
        printf("\n");*/

#define PUSH \
                    if ((stable || next.stable()) && dist.find(next) == dist.end()) { \
                        dist[next] = turns+1; \
                        que.push(next); \
                    } \

        for (int i = 0; i < H; i++) {
            if (cur.data[i] == 0)
                continue;
            for (int j = 0; j < W; j++) {
                if (!(cur.data[i] & (1<<j)))
                    continue;
                next.data[i] ^= 1 << j;

                // move up
                if (i != 0 && i != H-1 && !(cur.data[i-1] & (1<<j))  && !(cur.data[i+1] & (1<<j)) && board[i+1][j] != '#' && board[i-1][j] != '#') {
                    // move up
                    next.data[i-1] |= 1 << j;
                    PUSH;
                    next.data[i-1] ^= 1 << j;
                    // move down
                    next.data[i+1] |= 1 << j;
                    PUSH;
                    next.data[i+1] ^= 1 << j;
                }
                if (j != 0 && j != W-1 && !(cur.data[i] & (1<<(j+1))) && !(cur.data[i] & (1<<(j-1))) && board[i][j+1] != '#' && board[i][j-1] != '#') {
                    // move left
                    next.data[i] |= 1 << (j-1);
                    PUSH;
                    next.data[i] ^= 1 << (j-1);
                    // move right
                    next.data[i] |= 1 << (j+1);
                    PUSH;
                    next.data[i] ^= 1 << (j+1);
                }

                next.data[i] ^= 1 << j;
            }
        }

        if (dist.find(goal) != dist.end())
            return dist[goal];
    }
    return -1;
}

int main()
{
    int T;
    scanf("%d", &T);
    for (int i = 1; i <= T; i++) {
        printf("Case #%d: %d\n", i, solve());
    }
    return 0;
}
