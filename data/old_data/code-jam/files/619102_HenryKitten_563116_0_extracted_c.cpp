#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

bool Board[2048][2048];
bool Marked[2048][2048];
int Group[2048][2048];
int M,N;

char LineBuf[2048];

inline void reachAndSet(int r,int c,int cons){
	if(Marked[r][c]) return;

	int maxlen = 1;
	int nowlen;
	{
		int len = 1;
		for(int j=c+1;j<N;j++){
			if(Board[r][j] == Board[r][j-1] || Marked[r][j]) break;
			len++;
		}
		nowlen = len;
	}
	
//	printf("nowlen:%d\n",nowlen);
	for(int i=r+1;i<M;i++){
		int len = 1;
		if(Marked[i][c]) break;
		if(Board[i][c] == Board[i-1][c]) break;
		for(int j=c+1;j<N;j++){
			if(Board[i][j] != Board[i][j-1] && Board[i][j] != Board[i-1][j] && (!Marked[i][j])){}
			else break;
			len++;
		}
		nowlen = min(len,nowlen);
		maxlen = max(min(nowlen,i-r+1),maxlen);
		if(nowlen <= i-r+1) break;
	}

//	printf("%d %d\n",r,c);
	if(cons > maxlen) return;
	for(int i=r;i<r+maxlen;i++){
		for(int j=c;j<c+maxlen;j++){
			Marked[i][j] = 1;
		}
	}
	Group[r][c] = maxlen;
	//if(maxlen == 2) printf("%d %d\n",r,c);
}

int main(){
	int n;
	int ncas,cas;
	scanf("%d",&ncas);
	for(int cas=1;cas<=ncas;cas++){
		memset(Board,0,sizeof(Board));
		memset(Marked,0,sizeof(Marked));
		memset(Group,0,sizeof(Group));

		scanf("%d%d",&M,&N);
		for(int i=0;i<M;i++){
			scanf("%s",LineBuf);
			for(int j=0;j<(N>>2);j++){
				char ch = LineBuf[j];
				int code;
				if(ch <= '9' && ch >= '0') code = ch - '0';
				else code = ch - 'A' + 10;
				Board[i][j*4+0] = ((code&8)  > 0);		
				Board[i][j*4+1] = ((code&4)  > 0);		
				Board[i][j*4+2] = ((code&2)  > 0);		
				Board[i][j*4+3] = ((code&1)  > 0);		
			}
		}
/*	
		for(int i=0;i<M;i++){
			for(int j=0;j<N;j++){
				printf("%d",Board[i][j]);
			}
			putchar(10);
		}
*/
		for(int k=max(M,N);k>=0;k--){
			for(int i=0;i<M;i++){
				for(int j=0;j<N;j++){
					reachAndSet(i,j,k);
				}
			}
		}

		int nans = 0;
		int anspar[2049] = {0};
		for(int i=0;i<M;i++){
			for(int j=0;j<N;j++){
				if(Group[i][j]){
					if(anspar[Group[i][j]] == 0) nans++;
					anspar[Group[i][j]]++;
				}
			}
		}
				
		printf("Case #%d: %d\n",cas,nans);
		for(int i=2048;i>=1;i--){
			if(anspar[i]) printf("%d %d\n",i,anspar[i]);
		}
	}
	return 0;
}
