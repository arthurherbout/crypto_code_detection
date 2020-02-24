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
		/*Write Code Here*/
		int path[][2] = {{2,1},{1,2}};
		int h,w,r;
		bool obs[128][128] = {0};
		int method[128][128] = {0};
		cin >> h >> w >> r;
		for(int i=0;i<r;i++){
			int u,v;
			cin >> u >> v;
			obs[v-1][u-1] = 1;
		}
		queue<int> grid;
		grid.push(0);
		method[0][0] = 1;
		while(!grid.empty()){
			int x = grid.front() % w;
			int y = grid.front() / w;
			grid.pop();
			
			for(int i=0;i<2;i++){
				int xc = x + path[i][0];
				int yc = y + path[i][1];
				if(xc<w && yc<h && !obs[xc][yc]){
					if(method[xc][yc] == 0) grid.push(xc+yc*w);
					method[xc][yc] = (method[xc][yc] + method[x][y]) % 10007;
				}
			}
		}
		printf("Case #%d: %d\n",ti,method[w-1][h-1]%10007);
	}
	return 0;
}
