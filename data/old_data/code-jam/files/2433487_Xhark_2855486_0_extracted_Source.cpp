#include <algorithm>
#include <vector>
#include <map>
#include <set>

using namespace std;

#include <stdio.h>

int x[10],y[10];

int X[10], Y[10], tsol[10], sol[10];
bool chk[10];
int n;
double sol_area;
int CCW(int ax,int ay,int bx,int by,int cx,int cy) {
	return (ax * by + bx * cy + cx * ay) -
		(ay * bx + by * cx + cy * ax);
}
bool eq(int ax, int ay,int bx,int by) {
	return ax == bx && ay == by;
}
bool ccw(int ax,int ay,int bx, int by, int cx,int cy, int dx, int dy) {
	if (eq(ax,ay,cx,cy) || eq(ax,ay,dx,dy) ||
		eq(bx,by,cx,cy) || eq(bx,by,dx,dy)) return false;

	return CCW(ax, ay,bx,by,cx,cy) * CCW(ax,ay,bx,by,dx,dy) <= 0 && 
		CCW(cx, cy,dx,dy,ax,ay) * CCW(cx,cy,dx,dy,bx,by) <= 0;
}

void back(int t) {
	if (t == n) {
		double a1, a2;
		a1 = 0; a2 = 0;
		for (int i=0;i<n;i++) {
			int p = i+1;
			if (p >= n) p = 0;
			a1 += X[i] * Y[p];
			a2 += Y[i] * X[p];
			for (int j=0;j<n;j++) {
				int q = j+1;
				if (q >= n) q = 0;
				if (ccw(X[i], Y[i], X[p], Y[p], X[j], Y[j], X[q], Y[q])) 
					return;
			}
		}
		double u = a1 - a2;
		if (u < 0) u = -u;
		if (u > sol_area) {
			sol_area = u;
			for (int i=0;i<n;i++) sol[i] = tsol[i];
		}
		return;
	}
	for (int i=0;i<n;i++) {
		if (!chk[i]) {
			chk[i] = true;
			X[t] = x[i];
			Y[t] = y[i];
			tsol[t] = i;
			back(t+1);
			chk[i] = false;
		}
	}
}
int main() {
	freopen("B-small-attempt1.in","r",stdin);
	freopen("B-small-attempt1.out","w",stdout);

	int T;
	scanf("%d",&T);
	while (T-- > 0) {
		scanf("%d",&n);
		for (int i=0;i<n;i++) {
			scanf("%d %d",&x[i],&y[i]);
			chk[i] = false;
		}

		sol_area = -1.0;
		back(0);

		static int cs = 1;
		printf("Case #%d:", cs ++);
		for (int i=0;i<n;i++) printf(" %d", sol[i]);
		printf("\n");
	}
	return 0;
}