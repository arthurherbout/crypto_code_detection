#include <stdio.h>
char board[8][8];
int dr[8] = {1,0,-1,0,1,1,-1,-1};
int dc[8] = {0,-1,0,1,1,-1,1,-1};
int dp[16<<16];
int R, C;
int go(int r, int c, int mask){
    int key = ((r*R+c)<<16)|mask;
    if(dp[key])return dp[key];
    int rr, cc, i, j;
    for(i = 0; i<8; i++){
        rr = r+dr[i];
        cc = c+dc[i];
        if(cc < 0 || cc>=C || rr<0 || rr >= R || board[rr][cc] == '#')continue;
        int x = rr*R + cc;
        if((1<<x)&mask)continue;
        int g = go(rr,cc,mask|(1<<x));
        if(g == -1)
            return dp[key] = 1;
    }
    return dp[key] = -1;
}

int main(){
    int i, j, k, N, M, r,c;
    scanf("%d",&N);
    char buf[10];
    for(i = 1; i<=N; i++){
        scanf("%d%d",&R,&C);
        gets(buf);
        for(j = 0; j<R; j++){
            gets(board[j]);
            //printf("%s\n",board[j]);
            for(k = 0; k<C; k++){
                if(board[j][k] == 'K'){
                    r = j;c = k;
                }
            }
        }
        memset(dp,0,(sizeof(int)*16)<<16);
        int g = go(r,c,(1<<(r*R+c)));
        printf("Case #%d: %c\n",i,g ==-1?'A':'B');
    }
}
