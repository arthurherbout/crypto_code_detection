#include <iostream>
#include <cstdio>

using namespace std;

int main() {
	int T;
	scanf("%d", &T);
	for(int cas=1; cas<=T; ++cas) {
		long long n;
		int pd, pg;
		scanf("%lld%d%d", &n, &pd, &pg);
		bool flag = true;
		if(pg==100&&pd!=100)
			flag = false;
		if(pg==0&&pd!=0)
			flag = false;
		int dem = 100;
		while(dem%2==0&&pd%2==0) {
			pd /= 2;
			dem /= 2;
		}
		while(dem%5==0&&pd%5==0) {
			pd /= 5;
			dem /= 5;
		}
		if(dem>n)
			flag = false;
		printf("Case #%d: ", cas);
		if(flag) printf("Possible\n");
		else printf("Broken\n");
	}
}
