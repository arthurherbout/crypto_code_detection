#include <cstdio>

using namespace std;

int add(int a, int b){
	return a ^ b;
}

int n,max;
int val[1000];
int backtrack(int i, int s1, int sn1, int s2, int sn2){
	//printf("%d %d %d %d %d\n", i,s1,sn1,s2,sn2);
	if (i == n){
		if (s1 != s2 || sn1 == 0 || sn2 == 0) return -1;
		return (sn1 > sn2) ? sn1 : sn2;
	}
	int x1 = backtrack(i+1,add(s1,val[i]),sn1+val[i],s2,sn2);
	int x2 = backtrack(i+1,s1,sn1,add(s2,val[i]),sn2+val[i]);
	return (x1 > x2) ? x1 : x2;
}

int main(){
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
	
	int t;
	scanf("%d", &t);
	for (int tc = 1; tc <= t; ++tc){
		scanf("%d", &n);
		for (int i = 0; i < n; ++i) scanf("%d", &val[i]);
		int m = backtrack(0,0,0,0,0);
		
		if (m != -1) printf("Case #%d: %d\n", tc,m);
		else printf("Case #%d: NO\n", tc);
	}
	
	return 0;
}
