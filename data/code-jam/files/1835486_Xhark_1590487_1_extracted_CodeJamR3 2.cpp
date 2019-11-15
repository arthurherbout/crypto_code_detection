#include <stdio.h>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <string>


using namespace std;

//#define MAX_S 3000
#define MAX_S 3001
#define MAX_M 10001

#define MAX 999999999
const int pd[6][2] = {
	{0, 1},{ 1, 1},{ 1, 0},
	{0,-1},{-1,-1},{-1, 0}
};

int dat[ 2*MAX_S+1 ][ 2*MAX_S+1 ];
bool chk[12][ 2*MAX_S+1 ][ 2*MAX_S+1 ];
bool stu[12][ 2*MAX_S+1 ][ 2*MAX_S+1 ];

int X[MAX_M], Y[MAX_M];
int bfx[ (2*MAX_S+1)*(2*MAX_S+1) ];
int bfy[ (2*MAX_S+1)*(2*MAX_S+1) ];
int head;

int S, M;

bool inc(int t,int x,int y) {
	bool res = false;
	int i, j;

	if (stu[t][x][y]){
		res = true;
	}
	for(i=0;i<6;i++){
		res = res || chk[t][ x+pd[i][0] ][ y+pd[i][1] ];
	}
	if (res ){
		bfx[0] = x; bfy[0] = y;
		head = 1;

		for (i=0;i<head;i++){
			int &X = bfx[i]; int &Y = bfy[i];
			chk[t][X][Y] = true;
			for(j=0;j<6;j++){
				if (dat[ X+pd[j][0] ][ Y+pd[j][1] ] && !chk[t][ X+pd[j][0] ][ Y+pd[j][1] ] ){
					chk[t][ X+pd[j][0] ][ Y+pd[j][1] ] = true;
					bfx[head] = X+pd[j][0];
					bfy[head] = Y+pd[j][1];
					head ++;
				}
			}
		}
	}
	return res;
}

int rone, rmx;
bool rchk() {
	int i, j, k;

	int sx, sy;
	sx = 0; sy = 0;
	for(i=0;i<6;i++){
		for(j=0;j<S;j++) {
			dat[sx][sy] = 2;
			sx += pd[i][0];
			sy += pd[i][1];
		}
	}

	head = 0;
	sx = 1; sy = 1;
	for(i=0;i<6;i++){
		for(j=0;j<S-1;j++) {
			if (!dat[sx][sy]) {
				dat[sx][sy] = 2;
				bfx[head] = sx;
				bfy[head] = sy;
				head ++;
			}
			sx += pd[i][0];
			sy += pd[i][1];
		}
	}

	for (i=0;i<head;i++){
		int &X = bfx[i]; int &Y = bfy[i];
		for(j=0;j<6;j++){
			if (!dat[ X+pd[j][0] ][ Y+pd[j][1]]){
				bfx[head] = X+pd[j][0];
				bfy[head] = Y+pd[j][1];
				dat[ X+pd[j][0] ][ Y+pd[j][1]] = 2;
				head ++;
			}
		}
	}

	bool res = false;

	rone = 0;
	rmx = ((S * (S+1))/2 - S) * 6 + 1;
	for(k=1;k<=S;k++) {
		sx = k; sy = k;
		if (S == k && dat[sx][sy]) rone ++;
		for(i=0;i<6;i++){
			for(j=0;j<S-k;j++){
				if (dat[sx][sy]) rone ++;
				sx += pd[i][0];
				sy += pd[i][1];
			}
		}
	}
	return rone < rmx;
}

int rinc(int x,int y){
	dat[x][y] = 0;
	rone --;

	int i, j;
	bool flw;
	flw = false;
	for(i=0;i<6;i++){
		if (dat[ x + pd[i][0] ][ y + pd[i][1] ] == 2) {
			flw = true;
		}
	}

	if (flw) {
		bfx[0] = x; bfy[0] = y;
		head = 1;
		rone ++;
		dat[x][y] = 2;
		for(i=0;i<head;i++){
			for(j=0;j<6;j++){
				int X = bfx[i] + pd[j][0];
				int Y = bfy[i] + pd[j][1];

				if (!dat[X][Y]) {
					if (X == 0 || Y == 0){
						X=Y;
					}

					bfx[head] = X;
					bfy[head] = Y;
					dat[X][Y] = 2;

					head ++;
					rone ++;
				}
			}
		}
	}

	return rone < rmx;
}
int main(){/*
	freopen("input.txt","w",stdout);
	printf("20\n");
	{
		int i, j;
		for(i=0;i<20;i++){
		printf("3000 10000\n");
			for(j=1;j<=10000;j++){
				printf("%d %d\n", j/3000+1, j%3000+1);
			}
		}
	}
	return 0;*/
	//freopen("B-small-attempt1.in","r",stdin);
	//freopen("B-small-attempt1.out","w",stdout);
	freopen("B-large.in","r",stdin);
	freopen("B-large.out","w",stdout);
	//
	
	int T;	
	scanf("%d",&T);
	while(T-->0) {
		fprintf(stderr, "%d\n", T);

		scanf("%d %d",&S,&M);
		int i, j;
		memset(dat ,0, sizeof(dat));
		memset(chk, 0, sizeof(chk));
		memset(stu, 0, sizeof(stu));

		int sx, sy;
		sx = 1; sy = 1;
		for(i=0;i<6;i++){
			//chk[6+i][sx][sy] = true;
			stu[6+i][sx][sy] = true;
			for(j=0;j<S-1;j++) {
				sx += pd[i][0];
				sy += pd[i][1];
				if (j < S-2) 
					stu[i][sx][sy] = true;
			}
		}
		
		int	bridge, fork, ring;
		bridge = fork = ring = -1;
		for(i=0;i<M;i++){
			int x, y;
			scanf("%d %d",&x, &y);
			X[i] = x; Y[i] = y;
			if (!dat[x][y]) {
				dat[x][y] = true;

				int cc;
				if (fork == -1) {
					cc = 0;
					for(j=0;j<6;j++){
						if(inc(j, x, y))
							cc ++;
					}
					if (cc >= 3) fork = i+1;
				}
				
				if (bridge == -1) {
					cc = 0;
					for(j=6;j<12;j++){
						if(inc(j, x, y)) cc ++;
					}
					if (cc >= 2) bridge = i+1;
				}
			}
		}
		memset(dat, 0, sizeof(dat));
		for(i=0;i<M;i++){
			dat[ X[i] ][ Y[i] ] = 1;
		}
		if (rchk()) ring = M;
		for(i=M-1;i>=0;i--){
			if (rinc(X[i], Y[i])) {
				ring = i;
			}
		}
/*
		int s, e;
		s = 0; e = M-1;
		while(s<=e) {
			int m = (s+e)/2;
			memset(dat, 0, sizeof(dat));
			for(i=0;i<=m;i++){
				dat[ X[i] ][ Y[i] ] = true;
			}
			if (rchk()) {
				e = m-1;
			} else {
				s = m+1;
			}
		}
		if (e < M-1) ring = e+2;
		else ring = -1;
*/

		static int cs = 1;
		printf("Case #%d: ", cs ++);

		if (bridge == -1) bridge = MAX;
		if (fork == -1) fork = MAX;
		if (ring == -1) ring = MAX;

		if (bridge == MAX && fork == MAX && ring == MAX) {
			printf("none");
		} else if (bridge < fork && bridge < ring) {
			printf("bridge in move %d", bridge);
		} else if (fork < bridge && fork < ring) {
			printf("fork in move %d", fork);
		} else if (ring < bridge && ring < fork) {
			printf("ring in move %d", ring);
		} else if (bridge == fork && fork < ring) {
			printf("bridge-fork in move %d", bridge);
		} else if (bridge == ring && ring < fork) {
			printf("bridge-ring in move %d", bridge);
		} else if (fork == ring && ring < bridge) {
			printf("fork-ring in move %d", fork);
		} else {
			printf("bridge-fork-ring in move %d", ring);
		}
		printf("\n");

	}
	return 0;
}