#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;

bool hit[15][15][901][901];
//short best[15][15][901][901];      // 225*4+1
int N;
int RR, CC;
int sx, sy, ex, ey;
char buf[20][20];

// NESW
int dd[4][2] = { {-1,0}, {0,1}, {1,0}, {0,-1}};

// portal:
//   0 = no portal
//   1 + ((CC * x) + y) * 4 + d

int pack(int x, int y, int d) {
    return 1 + ((CC*x)+y)*4 + d;
}

void unpack(int portal, int &x, int &y, int &d) {
    if (portal == 0) {x=y=d=-1;}
    portal--;
    d = portal % 4;
    portal /= 4;
    y = portal % CC;
    portal /= CC;
    x = portal;
}

bool move(int px, int py, int pd, int x, int y, int d) {
    return px==x && py==y && pd==d;
}

int fire(int x, int y, int d) {
    int tx = x+dd[d][0], ty = y+dd[d][1];
    while (1) {
        if (tx < 0 || ty < 0 || tx >= RR || ty >= CC || buf[tx][ty] == '#') {
            return pack(tx-dd[d][0], ty-dd[d][1], d);
        }
        tx += dd[d][0];
        ty += dd[d][1];
    }
    return -1;
}

struct state {
    int x, y, a, b, m;
};
state make(int x, int y, int a, int b, int m) {
    state ret = {x,y,a,b,m};
    return ret;
}
bool operator < (const state &a, const state &b) {
    return a.m > b.m;
}

int main() {
    scanf("%d", &N);
    for (int it = 1; it <= N; it++) {
        scanf("%d %d", &RR, &CC);
        memset(hit, 0, sizeof(hit));

        for (int i = 0; i < RR; i++)
            scanf("%s", buf[i]);

        for (int i = 0; i < RR; i++)
            for (int j = 0; j < CC; j++) {
                if (buf[i][j] == 'O') sx = i, sy = j;
                else if (buf[i][j] == 'X') ex = i, ey = j;
            }

        printf("Case #%d: ", it);
        deque<state> Q;
        Q.push_back(make(sx,sy,0,0,0));

        while (Q.size()) {
            state R = Q.front(); Q.pop_front();

            if (R.x == ex && R.y == ey) {
                printf("%d\n", R.m);
                goto done;
            }

            //            best[R.x][R.y][R.a][R.b] = R.m;

            // fire the gun
            for (int i = 0; i < 4; i++) {
                int aa = fire(R.x,R.y,i);
                if (!hit[R.x][R.y][aa][R.b]) {
                    hit[R.x][R.y][aa][R.b] = 1;
                    Q.push_front(make(R.x,R.y,aa,R.b,R.m));
                }
                if (!hit[R.x][R.y][R.a][aa]) {
                //                if (R.m < best[R.x][R.y][R.a][aa]) {
                    hit[R.x][R.y][R.a][aa] = 1;
                    //                    best[R.x][R.y][R.a][aa] = R.m;
                    Q.push_front(make(R.x,R.y,R.a,aa,R.m));
                }
            }

            for (int i = 0; i < 4; i++) {
                int tx = R.x + dd[i][0];
                int ty = R.y + dd[i][1];

                // see if you can move through a portal
                if (R.a && R.b) {
                    int ax,ay,ad,bx,by,bd;
                    unpack(R.a,ax,ay,ad);
                    unpack(R.b,bx,by,bd);
                    if (move(ax,ay,ad,R.x,R.y,i)) {
                        if (!hit[bx][by][R.a][R.b]) {
                            hit[bx][by][R.a][R.b] = 1;
                            //                        if (R.m + 1 < best[bx][by][R.a][R.b]) {
                            //                            best[bx][by][R.a][R.b] = R.m + 1;
                            Q.push_back(make(bx, by, R.a, R.b, R.m + 1));
                        }
                    }
                    else if (move(bx,by,bd,R.x,R.y,i)) {
                        if (!hit[ax][ay][R.a][R.b]) {
                            //                        if (R.m + 1 < best[ax][ay][R.a][R.b]) {
                            //                            best[ax][ay][R.a][R.b] = R.m + 1;
                            hit[ax][ay][R.a][R.b] = 1;
                            Q.push_back(make(ax, ay, R.a, R.b, R.m + 1));
                        }                        
                    }
                }

                // otherwise
                if (tx < 0 || ty < 0 || tx >= RR || ty >= CC || buf[tx][ty] == '#') continue;
                if (!hit[tx][ty][R.a][R.b]) {
                //                if (R.m + 1 < best[tx][ty][R.a][R.b]) {
                    //                    best[tx][ty][R.a][R.b] = R.m + 1;
                    hit[tx][ty][R.a][R.b] = 1;
                    Q.push_back(make(tx,ty,R.a,R.b,R.m+1));
                }
            }
        }


        printf("THE CAKE IS A LIE\n");
    done:;
    }
}
