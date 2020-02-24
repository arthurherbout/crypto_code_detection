#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <string>
#include <set>
#include <map>


using namespace std;

int const MAXN = 2500;

int n;
int a[MAXN];
int b[MAXN];

set<int> maggiori[MAXN];
set<int> minori[MAXN];

int successione[MAXN];

void solve() {
	scanf("%d",&n);
	
	for (int i=0; i<n; ++i) {
		scanf("%d",a+i);
		
		maggiori[i].clear();
		minori[i].clear();
		successione[i] = -1;
	}
	for (int i=0; i<n; ++i) {
		scanf("%d",b+i);
	}
	
	// scorro A
	for (int i=0; i<n; ++i) {
		int k = a[i];
		int ultimo = -1;
		
		for (int j=0; j<i; ++j) {
			if ( a[j] >= k ) {
				// i è più basso di j
				// maggiori[i].insert(j);
				minori[j].insert(i);
			}
			if ( a[j] == k-1 ) {
				ultimo = j;
			}
		}
		
		assert( ( ultimo >= 0 ) || ( k == 1 ) );
		
		// i è più alto di ultimo
		if ( ultimo >= 0 ) minori[i].insert(ultimo);
		// maggiori[ultimo].insert(i);
	}
	
	// scorro B
	for (int i=n-1; i>=0; --i) {
		int k = b[i];
		int ultimo = -1;
		
		for (int j=n-1; j>i; --j) {
			if ( b[j] >= k ) {
				// i è più basso di j
				// maggiori[i].insert(j);
				minori[j].insert(i);
			}
			if ( b[j] == k-1 ) {
				ultimo = j;
			}
		}
		
		//printf("i=%d, k=%d, ultimo=%d\n",i,k,ultimo);
		
		assert( ( ultimo >= 0 ) || ( k == 1 ) );
		
		// i è più alto di ultimo
		if ( ultimo >= 0 ) minori[i].insert(ultimo);
		// maggiori[ultimo].insert(i);
	}
	
	// toposort
	
	for (int k=0; k<n; ++k) {
		// dove va k?
		
		int i=0;
		for (i=0; i<n; ++i) {
			if ( successione[i] != -1 ) continue;
			
			if ( minori[i].empty() ) {
				
				// posso metterlo in i
				
				successione[i] = k;
				
				// elimino i suoi archi
				for (int j=0; j<n; ++j) {
					minori[j].erase(i);
				}
				
				break;
			}
		}
		
		assert(i<n);
	}
	
	for (int i=0; i<n; ++i) {
		printf(" %d", successione[i]+1);
	}
	printf("\n");
}

int main() {
	
	int t;
	scanf("%d",&t);
	
	for (int i=0; i<t; ++i) {
		printf("Case #%d:",i+1);
		solve();
	}
	
	return 0;
}
