#include <iostream>
#include <algorithm>
#include "stdio.h"
using namespace std;

#define DBGOUT printf
//void DBGOUT(...) { } 

int bo[512][512];
int v[512][512];
int mm[512][512];
int M,N;
int made[512][2];
int made_cnt;
int tiles;

int pickLarge(int n)
{
	int count = 0;

//	DBGOUT("pick %d\n", n);

	int i ,j, k, m;
	for (i = 0 ; i < M-1;i++) {
		for (j =0 ;j < N-1;j++) {
			if (mm[i][j] == n) 
			{
				for (k= 0; k < n; k++) {
					for(m = 0; m < n; m++) {
						mm[i+k][j+m] = 0;
						bo[i+k][j+m] = -1;
					}
				}
				count++;
			}
		}
	}
	return count;
}

int visitBoard()
{
	int i, j;

	memset(v, 0, sizeof(int)*512*512);
	memset(mm, 0, sizeof(int)*512*512);

	for(i = 0 ;i < M-1; i++) {
		for (j  =0 ; j < N-1; j++) {
			if (bo[i][j] < 0 || bo[i][j+1] <0 || bo[i+1][j] < 0 || bo[i+1][j+1] < 0)
				continue;
			if (bo[i][j] != bo[i][j+1] && bo[i][j] != bo[i+1][j] && bo[i][j] == bo[i+1][j+1])
				v[i][j] = 1;
		}
	}

	for (i = 0 ; i < M-1; i++) {
		for( j = 0 ;j < N-1;j++) {
			if (v[i][j] > 0 && mm[i][j] == 0) {
				int k,m,pass=1,siz=0;
				for (k = 0;pass>0;k++) {
					if (i + k > M-1 || j +k > N-1)
						break;
					for(m=0; m <=k && pass>0; m++) {
						if (v[i+k][j+m] == 0)
							pass=0;
					}
					for(m=0; m <=k && pass>0; m++) {
						if (v[i+m][j+k] == 0)
							pass=0;
					}
					siz=k+1;
				}

				if (siz > 0)
					mm[i][j] = siz;
			}
		}
	}
}

int findSolution()
{
	int count = 0;
	int i,j;

	memset(made, 0, sizeof(int)*512*2);
	made_cnt = 0;





	while(1)
	{
		int max = -1;
		int count;

		visitBoard();

/*
	for ( i = 0 ; i < M; i++) {
		printf("=");
		for (j = 0; j < N; j++) {
			printf("%d ", bo[i][j]);
		}
		printf("===\n");
	}

		for ( i = 0 ; i < M; i++) {
			for (j = 0; j < N; j++) {
				printf("%2d ", mm[i][j]);
			}
			printf("\n");
		}
		printf("\n");

*/
		for ( i = 0 ; i < M; i++) 
			for (j = 0; j < N; j++) 
				if (mm[i][j] > max)
				max = mm[i][j];

		if (max < 1)
			break;

		// merge
		count = pickLarge(max);
		made[made_cnt][0] = max;
		made[made_cnt][1] = count;
		made_cnt++;

		tiles -= max*max*count;
		
/*
		for ( i = 0 ; i < M; i++) {
			for (j = 0; j < N; j++) {
				printf("%2d ", mm[i][j]);
			}
			printf("\n");
		}
*/
	}
					
	return made_cnt;
}


int main()
{
	int case_cnt;
	int ret = 0;

	cin >> case_cnt;

	for (int i = 0; i < case_cnt; i++)
	{
		char buf[512];
		int v;

		cin >> M >> N;

		for (int j = 0; j < M; j++) {
				cin >> buf;
				for (int k = 0; k < N/4; k++)
				{
					if (buf[k] >= 'A')
						v = buf[k] - 55;
					else
						v = buf[k] - '0';
					bo[j][k*4] = (v & 0x8) >> 3;
					bo[j][k*4+1] = (v & 0x4) >> 2;
					bo[j][k*4+2] = (v & 0x2) >> 1;
					bo[j][k*4+3] = v & 0x1;
				}
		}

/*
		for (int a = 0 ; a < M; a++) {
			for(int b =0 ; b< N; b++) {
				printf("%d ", bo[a][b]);
			}
			printf("\n");
		}
*/

		tiles = M*N;
		made_cnt = 0;
		ret = findSolution(); 
		if (tiles > 0) {
			made[made_cnt][0] = 1;
			made[made_cnt][1] = tiles;
			made_cnt++;
		}

		cout << "Case #" << i + 1 << ": " << made_cnt << endl;

		for (int k = 0; k < made_cnt; k++)
			cout << made[k][0] << " " << made[k][1] << endl;
	}
}

