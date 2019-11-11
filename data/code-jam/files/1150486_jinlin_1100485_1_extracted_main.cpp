#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

int ps[2000000];
bool f[2000001];

int main() {
	int T;
	scanf("%d", &T);
	memset(f, true, sizeof(f));
	int cnt = 0;
	for(int i=2; i<=2000000; ++i)
		if(f[i]) {
			ps[cnt++] = i;
			for(int j=i; i<=2000000/j; ++j)
				f[i*j] = false;
		}
	for(int cas=1; cas<=T; ++cas) {
		long long n;
		cin>>n;
		int ans=1;
		for(int i=0; i<cnt; ++i) {
			long long cur = ps[i];
			long long prd = cur;
			if(n/prd<cur) break;
			while(n/prd>=cur) {
				++ans;
				prd *= cur;
			}
		}
		if(n==1) ans = 0;
		printf("Case #%d: %d\n", cas, ans);
	}
}
