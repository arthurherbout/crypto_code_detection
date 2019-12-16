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

int main(void){
	int ncas,ti;
	cin >> ncas;
	for(ti=1;ti<=ncas;ti++){
		int p[4][2] = {0};
		/*Write Code Here*/
		int a,b,c,d;
		int N,M,A;
		bool flag = 0;
		cin >> N >> M >> A;
		for(a=0;a<=N;a++){
			for(b=0;b<=M;b++){
				for(c=0;c<=N;c++){
					for(d=0;d<=M;d++){
						int det = abs(a*d-b*c);
						if(det == A){
							flag = 1;
							break;	
						}
					}
					if(flag) break;
				}
				if(flag) break;
			}
			if(flag) break;
		}
		if(!flag) printf("Case #%d: IMPOSSIBLE\n",ti);
		else printf("Case #%d: %d %d %d %d %d %d\n",ti,0,0,a,b,c,d);
	}
	return 0;
}
