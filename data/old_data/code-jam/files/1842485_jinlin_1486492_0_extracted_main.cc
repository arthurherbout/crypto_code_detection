#include <algorithm>
#include <iostream>
#include <cstdio>

using namespace std;

int N;
int x[4000];
int y[4000];
bool flag;
int miny;

void solve(int l, int r, int dx, int dy) {
	//fprintf(stderr, "!!: %d %d\n", l, r);
	miny = min(miny, y[r]);
	long long tmp;
	int cnt=0;
	for(int i=l; i<r; ++i) {
		if(x[i]!=r) continue;
		++cnt;
		tmp = 1ll*dy*(r-i)/dx+2;
		y[i] = y[r]-(int)tmp;
		solve(l, i, r-i, (int)tmp);
		l = i+1;
	}
	if(l==r) return;
	if(cnt==0) flag = false;
}

int main() {
	int T;
	scanf("%d", &T);
	for(int cas=1; cas<=T; ++cas) {
		scanf("%d", &N);
		for(int i=1; i<N; ++i)
			scanf("%d", x+i);
		miny = 0;
		flag = true;
		y[N] = 0;
		solve(1, N, 1, 0);
		printf("Case #%d:", cas);
		if(!flag) {
			printf(" Impossible\n");
			continue;
		}
		//fprintf(stderr, "? %d\n", miny);
		for(int i=1; i<=N; ++i) {
			printf(" %d", y[i]-miny);
		}
		printf("\n");
	}
}
