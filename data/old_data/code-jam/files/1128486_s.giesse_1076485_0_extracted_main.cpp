#include <cstdio>

using namespace std;

int simple(int l, int h, int n, int* others){
	for (int i = l; i <= h; ++i){
		bool ok = true;
		for (int j = 0; j < n && ok; ++j){
			if (!(others[j] % i == 0 || i % others[j] == 0)) ok = false;
		}
		if (ok) return i;
	}
	return -1;
}

int main(){
	freopen("C-small-attempt0.in", "r", stdin);
	freopen("out.txt", "w", stdout);
	
	int T;
	scanf("%d", &T);
	for (int tc = 1; tc <= T; ++tc){
		printf("Case #%d: ", tc);
		int n,l,h;
		scanf("%d %d %d", &n, &l, &h);
		int others[n];
		for (int i = 0; i < n; ++i) scanf("%d", &others[i]);
		
		int a = simple(l,h,n,others);
		if (a == -1) printf("NO\n");
		else printf("%d\n", a);
	}
	
	return 0;
}
