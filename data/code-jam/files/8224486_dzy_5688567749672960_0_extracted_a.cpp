#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
typedef long long ll;
const int mo=1e9+7;
int pow(int a,int b,int c){int ret=1;for(;b;b>>=1,a=1LL*a*a%c)if(b&1)ret=1LL*ret*a%c;return ret;}

int f[1000010], q[1000010], rev[1000010];
int R(int x){
	int v = 0;
	while(x){
		v = v * 10 + x % 10;
		x /= 10;
	}
	return v;
}

void bfs(int n){
	for(int i = 1;i <= n;i ++) rev[i] = R(i);
	int s = 1, e = 1;
	q[1] = 1;
	f[1] = 1;
	while(s <= e){
		int u = q[s ++];
		if(u + 1 <= n && !f[u + 1]){
			f[u + 1] = f[u] + 1;
			q[++ e] = u + 1;
		}
		int t = rev[u];
		if(t <= n && !f[t]){
			f[t] = f[u] + 1;
			q[++ e] = t;
		}
	}
}
int main(){
	freopen("data1.in", "r", stdin);
	freopen("data1.out", "w", stdout);
	int tc;
	bfs(1e6);
	scanf("%d", &tc);
	int T = 0, n;
	while(tc --){
		cin >> n;
		printf("Case #%d: %d\n", ++ T, f[n]);
	}
	return 0;
}



