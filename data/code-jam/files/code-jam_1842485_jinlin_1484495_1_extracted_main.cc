#include <algorithm>
#include <cstdio>

using namespace std;

struct Student {
	int r, i;
	double x, y;
} s[2000];

bool cmpByR(const Student& sI, const Student& sII) {
	return sI.r>sII.r;
}

bool cmpByI(const Student& sI, const Student& sII) {
	return sI.i<sII.i;
}

int getE(int x) {
	int i;
	for(i=0; (1<<i)<x*2; ++i);
	return i;
}

int cur;
int N, W, L;

void fromSomeWhere(int x, int y, int preE) {
	if(cur>=N) return;
	if(getE(s[cur].r)==preE) {
		if(y+s[cur].r>L) return;
		s[cur].y = y+s[cur].r;
		s[cur].x = x+(1<<(preE-1));
		fromSomeWhere(x, y+s[cur++].r*2, preE);
	} else {
		fromSomeWhere(x, y, preE-1);
		fromSomeWhere(x+(1<<(preE-1)), y, preE-1);
	}
}

void fromLeft() {
	int curx = 0;
	while(cur<N) {
		int e = getE(s[cur].r);
		s[cur].y = 0;
		s[cur].x = (1<<(e-1))+curx;
		++cur;
		fromSomeWhere(curx, 1<<(e-1), e);
		curx += (1<<e);
	}
}

int main() {
	int T;
	scanf("%d", &T);
	for(int cas=1; cas<=T; ++cas) {
		scanf("%d%d%d", &N, &W, &L);
		int sumR = 0;
		for(int i=0; i<N; ++i) {
			scanf("%d", &s[i].r);
			s[i].i = i;
			sumR += s[i].r;
		}
		printf("Case #%d:", cas);
		if(sumR*2<=L) {
			int curR = 0;
			for(int i=0; i<N; ++i) {
				printf(" %.1lf %.1lf", 0.0, 0.0+curR+s[i].r);
				curR += s[i].r*2;
			}
			printf("\n");
			continue;
		}
		sort(s, s+N, cmpByR);
		cur = 0;
		fromLeft();
		sort(s, s+N, cmpByI);
		for(int i=0; i<N; ++i) {
			printf(" %.1lf %.1lf", s[i].x, s[i].y);
		}
		printf("\n");
	}
}

