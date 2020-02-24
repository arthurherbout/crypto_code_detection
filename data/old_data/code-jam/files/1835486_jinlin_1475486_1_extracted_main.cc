#include <algorithm>
#include <cstring>
#include <cstdio>

using namespace std;

int k[20000];
int p[20000];
int l[20000];

bool cmp(int x, int y) {
	if(l[x]*(1-p[x])==l[y]*(1-p[y])) return x<y;
	return (l[x]*(1-p[x])<l[y]*(1-p[y]));
}

int main() {
	int t, n;
	scanf("%d", &t);
	for(int cas=1; cas<=t; ++cas) {
		scanf("%d", &n);
		for(int i=0; i<n; ++i)
			scanf("%d", l+i);
		for(int i=0; i<n; ++i)
			scanf("%d", p+i);
		for(int i=0; i<n; ++i)
			k[i] = i;
		sort(k, k+n, cmp);
		printf("Case #%d:", cas);
		for(int i=0; i<n; ++i)
			printf(" %d", k[i]);
		printf("\n");
	}
	return 0;
}
