#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

double vs[2000];
double ls[2000];
int ind[2000];

bool cmp(int a, int b) {
	return vs[a]<vs[b];
}

int main() {
	int T;
	scanf("%d", &T);
	for(int cas=1; cas<=T; ++cas) {
		double X, S, R, t;
		int N;
		scanf("%lf%lf%lf%lf%d", &X, &S, &R, &t, &N);
		if(R<S) R = S;
		double len = X;
		for(int b, e, w, i=0; i<N; ++i) {
			scanf("%d%d%d", &b, &e, &w);
			ls[i] = e-b;
			len -= ls[i];
			vs[i] = w;
		}
		printf("Case #%d: ", cas);
		if(t*R<=len) {
			double ans = t;
			ans += (len-t*R)/S;
			for(int i=0; i<N; ++i) {
				ans += ls[i]/(vs[i]+S);
			}
			printf("%.7lf\n", ans);
		} else {
			double ans = len/R;
			t -= ans;
			for(int i=0; i<N; ++i)
				ind[i] = i;
			sort(ind, ind+N, cmp);
			for(int index=0; index<N; ++index) {
				int i = ind[index];
				if(t*(vs[i]+R)>ls[i]) {
					t -= ls[i]/(vs[i]+R);
					ans += ls[i]/(vs[i]+R);
				} else {
					ans += t;
					ans += (ls[i]-t*(vs[i]+R))/(vs[i]+S);
					t = 0;
				}
			}
			printf("%.7lf\n", ans);
		}
	}
}
