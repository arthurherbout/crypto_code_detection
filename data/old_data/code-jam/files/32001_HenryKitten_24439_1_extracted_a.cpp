#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <queue>

//c lib
#include <cmath>
#include <cstring>
#include <cstdlib>

//alias
#define ll long long
#define lmax 1000000000000000ll

//user defined lib
#include <primes.h>

using namespace std;

int main(void){
	int ncas,ti;
	cin >> ncas;
	for(ti=1;ti<=ncas;ti++){
		/*Write Code Here*/
		ll tree[10001][2];
		ll gate[10001];
		bool change[10001];
		int M,V;
		cin >> M >> V;
		for(int i=0;i<(M-1)/2;i++){
			cin >> gate[i] >> change[i];	
		}
		for(int i=(M-1)/2;i<M;i++){
			cin >> gate[i];	
			tree[i][gate[i]] = 0;
			tree[i][1-gate[i]] = lmax;
		}

		for(int i=(M-1)/2-1;i>=0;i--){
			int l = i*2+1;
			int r = i*2+2;
			if(gate[i] == 1){
				tree[i][1] = tree[l][1] + tree[r][1];
				tree[i][0] = min(tree[l][1] + tree[r][0], min(tree[l][0] + tree[r][1], tree[l][0]+tree[r][0]));
				if(change[i]){
					tree[i][1] = min(tree[i][1], min(tree[l][1] + tree[r][0], min(tree[l][0] + tree[r][1], tree[l][1]+tree[r][1])) + 1);
					tree[i][0] = min(tree[i][0] ,tree[l][0] + tree[r][0] + 1);
				}
			}else{
				tree[i][1] = min(tree[l][1] + tree[r][0],min(tree[l][0] + tree[r][1], tree[l][1]+tree[r][1]));
				tree[i][0] = tree[l][0] + tree[r][0];
				if(change[i]){
					tree[i][1] = min(tree[i][1],tree[l][1] + tree[r][1] + 1);
					tree[i][0] = min(tree[i][0],min(tree[l][1] + tree[r][0], min(tree[l][0] + tree[r][1], tree[l][0]+tree[r][0])) + 1);	
				}
			}
			if(tree[i][1] >= lmax) tree[i][1] = lmax;
			if(tree[i][0] >= lmax) tree[i][0] = lmax;
		}
		
		printf("Case #%d: ",ti);
		if(tree[0][V] >= lmax) printf("IMPOSSIBLE\n");
		else printf("%lld\n",tree[0][V]);	
	}
	return 0;
}
