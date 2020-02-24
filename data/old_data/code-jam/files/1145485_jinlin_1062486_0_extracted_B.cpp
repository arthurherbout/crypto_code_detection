#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

char s[20001][11];
char l[27];
int p[20001], key[20001];
int ans, pnts;

bool cmp(int a, int b) {
	return key[a]<key[b];
}

int test(char *ch, char op) {
	if(*ch) {
		int nxt = test(ch+1, op);
		nxt <<= 1;
		nxt |= (*ch==op);
		return nxt;
	}
	return 0;
}

void find(int low, int high, int k, int pre) {
	//printf("!!: %d-%d : %c, %d \n ", low, high, l[k?k-1:k], pre);
	for(int i=low; i<high; ++i) {
		//printf("\t%s\n", s[p[i]]);
	}
	if(pre>pnts||(pre==pnts&&p[low]<ans)) {
		pnts = pre;
		//printf("!!: %s %d\n", s[p[low]], pre);
		ans = p[low];
	}
	if(high-low<=1) {
		//printf("!!: %c .. %s %d\n", l[k], s[p[low]], pre);
		return;
	}
	bool flag = true;
	while(flag) {
		for(int i=low; i<high; ++i) {
			key[p[i]] = test(s[p[i]], l[k]);
			//printf("!!: %d->%d\n", p[i], key[p[i]]);
			if(key[p[i]]) flag = false;
		}
		++k;
	} 
	--k;
	sort(p+low, p+high, cmp);
	for(int i=low, j=low; i<high; i=j) {
		while(j<high&&key[p[j]]==key[p[i]])
			++j;
		find(i, j, k+1, pre+!key[p[i]]);
	}
}

int main() {
	int T;
	scanf("%d", &T);
	for(int cas=1; cas<=T; ++cas) {
		int n, m;
		scanf("%d%d", &n, &m);
		for(int i=0; i<n; ++i)
			scanf("%s", s[i]);
		printf("Case #%d:", cas);
		for(int i=0; i<m; ++i) {
			scanf("%s", l);
			pnts = -1;
			ans = -1;
			for(int j=0; j<n; ++j) {
				p[j] = j;
				key[j] = strlen(s[j]);
			}
			sort(p, p+n, cmp);
			for(int j=0, k=0; j<n; j=k) {
				while(k<n&&key[p[k]]==key[p[j]])
					++k;
				find(j, k, 0, 0);
			}
			printf(" %s", s[ans]);
		}
		printf("\n");
	}
}
