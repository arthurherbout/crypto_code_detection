#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <string>
#include <cstring>
#include <ctime>
using namespace std;

char maze[100][100];
int H, W, D, dx, dy, ndx, ndy;
int startX, startY;
set <pair <int, int> > S;
bool used[150][150];

int gcd(int a, int b) {
    return !b ? a : gcd(b, a % b);
}

void rec(int atX, int atY, int realX, int realY, int &res) {
    if (atX < 0 || atX >= 150 || atY < 0 || atY >= 150 || used[atX][atY])
        return;
    used[atX][atY] = true;
    int deltaX = realX - startX, deltaY = realY - startY;
    if (deltaX * deltaX + deltaY * deltaY <= D * D) {
        cout << atX << " " << atY << " " << realX << " " << realY << endl;
        if (deltaX != 0 || deltaY != 0) {
            int D = gcd(abs(deltaX), abs(deltaY));
            deltaX /= D;
            deltaY /= D;

            if (S.find(make_pair(deltaX, deltaY)) == S.end()) {
                S.insert(make_pair(deltaX, deltaY));
                ++res;
             }
        }
    }
    rec(atX - 1, atY, realX - ndx, realY, res);
    rec(atX + 1, atY, realX + dx, realY, res);
    rec(atX, atY - 1, realX, realY - ndy, res);
    rec(atX, atY + 1, realX, realY + dy, res);
}
int main() {
freopen("D-small-attempt0.in", "r", stdin);
    freopen("D.out", "w", stdout);
    int T;
    scanf("%d", &T);
    for (int caseId = 1; caseId <= T; ++caseId) {
        scanf("%d %d %d", &H, &W, &D);
        for (int i = 0; i < H; ++i)
            scanf("%s", maze[i]);

        for (int i = 0; i < H; ++i)
            for (int j = 0; j < W; ++j)
                if (maze[i][j] == 'X') {
                    startX = i;
                    startY = j;
                    break;
                }
        memset(used, false, sizeof(used));
        int res = 0;
        H <<= 1;
        W <<= 1;
        D <<= 1;
        startX = startX * 2 + 1;
        startY = startY * 2 + 1;
        dx = W - 2 - startY;
        dy = H - 2 - startX;
        ndx = W - 4 - dx;
        ndy = H - 4 - dy;
        dx <<= 1;
        dy <<= 1;
        ndx <<= 1;
        ndy <<= 1;
        S.clear();
        for (int i = 0; i < 150; ++i)
            for (int j = 0; j < 150; ++j) {
                if (i == 75 && j == 75)
                    continue;
                int nX = i - 75, nY = j - 75;
                int curX = nX / 2 * (ndx + dx), curY = nY / 2 * (ndy + dy);
                if (nX % 2) {
                    if (nX < 0) {
                        curX -= ndx;
                    } else {
                        curX += dx;
                    }
                }
                if (nY % 2) {
                    if (nY < 0) {
                        curY -= ndy;
                    } else {
                        curY += dy;
                    }
                }
                int deltaX = curX, deltaY = curY;
                if (deltaX * deltaX + deltaY * deltaY <= D * D) {
                    if (deltaX != 0 || deltaY != 0) {
                        int D = gcd(abs(deltaX), abs(deltaY));
                        deltaX /= D;
                        deltaY /= D;

                        if (S.find(make_pair(deltaX, deltaY)) == S.end()) {
                            S.insert(make_pair(deltaX, deltaY));
                            ++res;
                         }
                    }
                }
            }
       // rec(75, 75, startX, startY, res);
        //cerr << caseId << endl;
        printf("Case #%d: %d\n", caseId, res);
    }
    return 0;
}
