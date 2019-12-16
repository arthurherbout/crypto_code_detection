#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <deque>
#include <string>
#include <set>
#include <map>


using namespace std;

int const MAXN = 1002;

int n;
int sigma[MAXN];

void solve() {
	scanf("%d", &n);
	
	for (int i=0; i<n; ++i) {
		scanf("%d", sigma+i);
	}
	
	int azzeccati = 0;
	
	for (int i=n-2; i>=0; i--) {
		for (int j=0; j<n/2; j++) {
			if ( sigma[i] == (i+j)%n ) azzeccati++;
		}
		
	}
	
	if ( azzeccati >= n/2 + 20 ) printf("BAD\n");
	else printf("GOOD\n");
}

int main() {
	
	int t;
	scanf("%d",&t);
	
	for (int i=0; i<t; ++i) {
		printf("Case #%d: ",i+1);
		solve();
	}
	
	return 0;
}
