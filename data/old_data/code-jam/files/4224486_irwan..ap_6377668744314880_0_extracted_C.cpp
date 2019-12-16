#include <iostream>
#include <vector>
using namespace std;

struct point {
    int x;
    int y;
};

long long int squareDistance(point p, point q) {
    long long int dx = p.x - q.x;
    long long int dy = p.y - q.y;
    return dx*dx + dy*dy;
}

int cross(point pivot, point p, point q) {
    point dq;
    dq.x = q.x - pivot.x;
    dq.y = q.y - pivot.y;
    point dp;
    dp.x = p.x - pivot.x;
    dp.y = p.y - pivot.y;
    int cross = dp.x*dq.y - dp.y*dq.x;
    return cross;
}

bool isAtBorder(const vector<point>& pts, int activeBitmap, int which) {
    int n = pts.size();
    vector<bool> active(n);
    for (int i = 0; i < n; i++) {
        active[i] = ((1<<i)&activeBitmap) > 0;
    }
    vector<bool> used(n, false);

    // Leftmost point
    int starting = -1;
    int minLeft = 1000000000;
    for (int i = 0; i < n; i++) {
        if (!active[i]) {
            continue;
        }
        if (pts[i].x < minLeft) {
            minLeft = pts[i].x;
            starting = i;
        }
    }
    if (starting == which) {
        return true;
    }
    int current = starting;
    point p = pts[starting];
    point q = p;
    q.y++;
    while (true) {
        // Find unused point that is closest to current point
        // and minimum angle
        int index = -1;
        for (int i = 0; i < n; i++) {
            if (i != current && active[i] && !used[i]) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            break;
        }
        for (int i = index+1; i < n; i++) {
            if (i != current && active[i] && !used[i]) {
                bool considered = cross(p, pts[i], q) >= 0;
                if (considered) {
                    int c = cross(p, pts[index], pts[i]);
                    if (c > 0) {
                        index = i;
                    } else {
                        if (c == 0) {
                            if (squareDistance(p, pts[i]) < squareDistance(p,pts[index])) {
                                index = i;
                            }
                        }
                    }
                }
            }
        }
        p = pts[index];
        q = p;
        q.x += (pts[index].x-pts[current].x);
        q.y += (pts[index].y-pts[current].y);
        current = index;
        used[current] = true;
        if (current == which) {
            return true;
        }
        if (current == starting) {
            break;
        }
    }
    return false;
}

int solve(const vector<point>& pts, int which) {
    int n = pts.size();
    int minCutDown = n;
    for (int activeBitmap = 0; activeBitmap < (1<<n); activeBitmap++) {
        if ((activeBitmap & (1<<which)) == 0) {
            continue;
        }
        int cutDown = 0;
        for (int i = 0; i < n; i++) {
            if ((activeBitmap & (1<<i)) == 0) {
                cutDown++;
            }
        }
        if (cutDown < minCutDown) {
            if (isAtBorder(pts, activeBitmap, which)) {
                minCutDown = cutDown;
            }
        }
    }
    return minCutDown;
}

int main() {

    /*
    vector<point> points(5);
    points[0].x = 0; points[0].y = 0;
    points[1].x = 10; points[1].y = 0;
    points[2].x = 10; points[2].y = 10;
    points[3].x = 0; points[3].y = 10;
    points[4].x = 5; points[4].y = 5;
    return isAtBorder(points, 255, 4);
    */

    int T;
    cin >> T;
    for (int t = 1; t <= T; t++) {
        int N;
        cin >> N;
        vector<point> pts(N);
        for (int i = 0; i < N; i++) {
            cin >> pts[i].x >> pts[i].y;
        }
        cout << "Case #" << t << ":" << endl;
        for (int i = 0; i < N; i++) {
            cout << solve(pts, i) << endl;
        }
    }

    return 0;
}

