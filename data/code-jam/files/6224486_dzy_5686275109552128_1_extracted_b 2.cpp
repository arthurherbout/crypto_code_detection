#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
int n, a[1005];
int main(){
	freopen("B-large.in", "r", stdin);
	freopen("data.out", "w", stdout);
	int tc, Tc = 0;
	scanf("%d", &tc);
	while(tc --){
		scanf("%d", &n);
		int mx = 0, ret = 1e9;
		for(int i = 1;i <= n;i ++) scanf("%d", &a[i]), mx = max(mx, a[i]);
		for(int i = 1;i <= mx;i ++){
			int tmp = i;
			for(int j = 1;j <= n;j ++) tmp += (a[j] - i) / i + ((a[j] - i) % i > 0);
			ret = min(ret, tmp);
		}
		printf("Case #%d: %d\n", ++ Tc, ret);
	}
	return 0;
}

