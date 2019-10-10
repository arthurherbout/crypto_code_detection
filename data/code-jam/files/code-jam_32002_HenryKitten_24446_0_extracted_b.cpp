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

//user defined lib
#include <primes.h>

using namespace std;

int best[2048][10];

bool isValid(int n){
	bool b41 = 0;
	while(n){
		if((n&1) && (b41 || ((n/2) & 1))) return 0;
		b41 = n&1;
		n/=2;
	}
	return 1;
}

void print(int n,int m){
	for(int j=0;j<m;j++){
	for(int i=0;i<1<<n;i++){
		printf("%d ",best[i][j]);
	}
	cout << endl;
	}
}

int main(void){
	int ncas,ti;
	cin >> ncas;
	for(ti=1;ti<=ncas;ti++){
		/*Write Code Here*/
		int m,n;
		char map[16][16];
		cin >> m >> n;
		for(int i=0;i<m;i++) scanf("%s",map[i]);
		memset(best,0,2048*10*4);
		for(int i=0;i<1<<n;i++){//m-1
			int nstu,idx;
			int dupi = i;
			if(!isValid(i)){
				best[i][m-1] = -INT_MAX;
				continue;
			}
			for(idx=nstu=0;dupi;dupi/=2,idx++){
				if((dupi&1) && map[m-1][idx] == 'x'){
					nstu = -INT_MAX;
					break;
				}
				nstu += dupi & 1;
			}
			best[i][m-1] = nstu;
		}
		for(int h=m-2;h>=0;h--){
			for(int i=0;i<1<<n;i++){
				int nstu,idx;
				int dupi = i;
				bool hasman[16] = {0};
				bool flag = 0;
				if(!isValid(i)){
                                	best[i][h] = -INT_MAX;
                               		continue;
                        	}
				for(idx=nstu=0;dupi;dupi/=2,idx++){
					if((dupi&1) && map[h][idx] == 'x'){
						flag = 1;
						break;
					}
					nstu += dupi & 1;
					hasman[idx] = 1;
				}
				if(flag){
					best[i][h] = -INT_MAX;
					continue;
				}
				int maxi = 0;
				for(int j=0;j<1<<n;j++){
					int dupj,jdx;
					bool die;
					if(best[j][h+1] == -INT_MAX) continue;
					for(die=jdx=0;dupj;dupj/=2,jdx++){
						if((dupj & 1) && ((jdx>0 && hasman[jdx-1]) || (jdx<n-1 && hasman[jdx+1]))){
							die = 1;
							break;
						}
					}
					//cout << j << " " << i << " " << die << endl;
					//cout << best[h+1][j] << endl;
					if((!die) && (maxi < best[j][h+1])) maxi = best[j][h+1];
				}
				best[i][h] = nstu + maxi;
			}
		}
		int maxi = 0;
		for(int i=0;i<1<<n;i++){
			if(maxi < best[i][0]) maxi = best[i][0];
		}
		printf("Case #%d: %d\n",ti,maxi);
	}
	return 0;
}
