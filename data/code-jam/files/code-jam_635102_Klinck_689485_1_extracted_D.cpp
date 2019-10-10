#include <stdio.h>
#include <math.h>

#define MOD 100003

int N;
int M;
int cowX[10000];
int cowY[10000];
int poleX[10000];
int poleY[10000];
double area[10000];

int convexhull[10000];
int hull_len;

void build_convex_hull() {
    hull_len = 0;
    int pos_first = 0;
    for(int i = 1; i<N; i++) {
        if (cowX[i]<cowX[pos_first]) {
            pos_first = i;
        } else if ((cowX[i] == cowX[pos_first]) && (cowY[i]>cowY[pos_first])) {
            pos_first = i;
        }
    }
    convexhull[1] = pos_first;
    hull_len ++;
    int vx = 0;
    int vy = 1;
    int pos = pos_first;
    while(true) {
        //printf("%d\n", pos);
        double best = -2.;
        int next = 0;
        double len0 = sqrt((double)(vx*vx+vy*vy));
        for(int i = 0; i<N; i++) {
            int ux = cowX[i] - cowX[pos];
            int uy = cowY[i] - cowY[pos];
            if ((ux==0)&&(uy==0)) continue;
            double len = sqrt((double)(ux*ux + uy*uy));
            double c = (ux*vx+uy*vy)/len/len0;
            if (c>best) {
                best = c;
                next = i;
            }
        }
        vx = cowX[next] - cowX[pos];
        vy = cowY[next] - cowY[pos];
        
        convexhull[hull_len+1] = next;
        pos = next;
        if (pos==pos_first) break;
        hull_len ++;
    }
    convexhull[0] = convexhull[hull_len];
}

double PI = acos(0)*2;

double slice(double a) {
    return a/2 - sin(a)/2;
}

double compute(int x, int y) {
    double sum = 0;
    for(int i = 1; i<=hull_len; i++) {
        int curr = convexhull[i];
        if ((cowX[curr]==x) && (cowY[curr]==y)) return 0;
    }
    for(int i = 1; i<=hull_len; i++) {
        int prev = convexhull[i-1];
        int curr = convexhull[i];
        int next = convexhull[i+1];
        int vx = x-cowX[curr];
        int vy = y-cowY[curr];
        double rad = sqrt((double)(vx*vx+vy*vy));
        int vxprev = cowX[prev] - cowX[curr];
        int vyprev = cowY[prev] - cowY[curr];
        int vxnext = cowX[next] - cowX[curr];
        int vynext = cowY[next] - cowY[curr];
        double cprev = (vx*vxprev + vy*vyprev) / rad / sqrt((double)(vxprev*vxprev+vyprev*vyprev));
        double cnext = (vx*vxnext + vy*vynext) / rad / sqrt((double)(vxnext*vxnext+vynext*vynext));
        int orprev = vx * vyprev - vy * vxprev;
        int ornext = vx * vynext - vy * vxnext;
        double aprev = acos(cprev);
        if (orprev<0) aprev = 2*PI - aprev;
        double anext = acos(cnext);
        if (ornext>0) anext = 2*PI - anext;
        double killprev = 2*PI - 2*aprev;
        double killnext = 2*PI - 2*anext;
        if (killprev<0) killprev = 0;
        if (killnext<0) killnext = 0;
        double piece = rad * rad *(PI- slice(killprev) - slice(killnext));
        if (piece<0) piece = 0;
        sum += piece;
    }
    return sum;
}

void solve() {
    build_convex_hull();
    for(int i = 0; i<M; i++) {
        area[i] = compute(poleX[i], poleY[i]);
    }
}

int main() {
    int T;
    scanf("%d", &T);
    for(int i = 0; i<T; i++) {
        scanf("%d %d", &N, &M);
        for(int j = 0; j<N; j++) {
            scanf("%d %d", &(cowX[j]), &(cowY[j]));
        }
        for(int j = 0; j<M; j++) {
            scanf("%d %d", &(poleX[j]), &(poleY[j]));
        }
        solve();
        printf("Case #%d:", i+1);
        for(int j = 0; j<M; j++) {
            printf(" %.10lf", area[j]);
        }
        printf("\n");
    }
            
    return 0;
}
