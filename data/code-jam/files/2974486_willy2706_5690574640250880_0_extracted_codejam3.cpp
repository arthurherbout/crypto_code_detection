#include <cstdio>
#include <algorithm>
#include <cstring>
#include <string>
#include <iostream>
#include <cmath>
#include <map>
#include <set>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <list>
#include <utility>
#include <bitset>
using namespace std;

int GCD (int a, int b ) {
	if ( a==0 ) return b;
	return GCD ( b%a, a );
}

#define CetakInt(J) printf("Cetak %d\n",J);
#define CetakChar(J) printf("%c\n",J);
#define sf scanf
#define pf printf
#define FOR(a,b,c) for(int a = b; a<=c ; a++)
#define FOR1(a,b,c) for(int a = b; a<c; a++)

typedef long long int int64;

bool dsc (int i,int j) { 
	return (i>j); 
}

int R,C,M;

int main(){
	int T;
	scanf("%d",&T);
	for (int i = 1; i <= T; ++i) {
		bool arr[51][51];
		memset(arr,0,sizeof(arr));
		scanf("%d %d %d",&R,&C,&M);
		//printf("ini %d %d %d\n",R,C,M);
		
		if (R == 1 || C == 1) {
			if (M > R*C - 2) {
				printf("Case #%d:\nImpossible\n",i);
				goto end;
			}
		} else {
			if (M > R*C - 4) {
				printf("Case #%d:\nImpossible\n",i);
				goto end;
			}
		}
		
		printf("Case #%d:\n",i);
		for (int maks = 1; M > 0; ++maks) {
			for (int row = 1; row <= max(maks,R) && M > 0; ++row) {
				for (int col = 1; col <= max(maks,C) && M > 0; ++col) {
					//printf("maxnya bro %d\n", max(maks,C));
					//printf("%d %d\n",row,col);
					if (row == R-1 && col == C-1) continue;
					if (row == R-1 && col == C) continue;
					if (row == R && col == C-1) continue;
					if (row == R && col == C) continue;
					if (col != maks && row != maks) continue;
					if (arr[row][col]) continue;
					arr[row][col] = 1;
					--M;
				}
				//printf("\n");
			}
		}

		for (int row = 1; row <= R; ++row) {
			for (int col = 1; col <= C; ++col) {
				if (row == R && col == C) {
					printf("c");
					continue;
				}
				if (arr[row][col])
					printf("*");
				else printf(".");
			}
			printf("\n");
		}
		
		end:
		printf("");
	}
	return 0;
}