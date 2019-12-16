#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>

using namespace std;

int T, N, M, K;

int main(void){
	FILE* fin = fopen("C-small-attempt1.in","r");
	FILE* fout = fopen("C-small-attempt1.out","w");

	int cases = 0, i, j, k;
	fscanf(fin, "%d", &T);
	while( cases++ < T ){
		/*Input*/
		fscanf(fin, "%d%d%d", &N, &M, &K);
		/*Solve*/
		int mini = 100000000;
		for(i=1; i<=N; i++){
			for(j=1; j<=M; j++){
				int margin = ( (i>2)? 2 : 0 )+( (j>2)? 2 : 0 );
				for(k=0; k<=margin; k++){
					int enclose = i*j+k-( (i>2)? 2 : 0 )-( (j>2)? 2 : 0 );
	//fprintf(fout,"i=%d j=%d enclose=%d K=%d",i,j,enclose,K);
					if( enclose>=K ){
						int stone = k+2*( (i>2)? i-2: i ) + 2*( (j>2)? j-2: j );
						if( mini > stone ){
							mini = stone;
						}
	//fprintf(fout," stone=%d",stone);
					}
	//fprintf(fout,"\n");
				}
			}
		}
		/*Output*/
		fprintf(fout, "Case #%d: ", cases);
		fprintf(fout, "%d\n", mini);
	}

	return 0;
}
