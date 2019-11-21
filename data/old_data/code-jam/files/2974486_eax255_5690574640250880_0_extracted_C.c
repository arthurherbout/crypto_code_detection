#include <stdio.h>
#include <string.h>
char c[100][100];
char cd[10000];
int chkvvvr;
int chkvvv(int R,int C,int M){
	int i,j;
	chkvvvr=0;
	memset(c,'.',10000);
	for(i=0;i<R;i++)c[i][C]=0;
	i=M%C;
	j=M/C;
	if(j==R-1){chkvvvr=1;return 0;}
	if(j==R-2&&(i&1)){chkvvvr=2;return 0;}
	if(i==C-1&&(R-2!=j)){chkvvvr=3;return 0;}
	for(i=0;i<j;i++)memset(c[i],'*',C);
	if(j==R-2){
		j=M-j*C;j>>=1;
		if(C-j<2){chkvvvr=4;return 0;}
		memset(c[i],'*',j);
		memset(c[i]+j,'.',C-j);
		i++;
		memset(c[i],'*',j);
		memset(c[i]+j,'.',C-j);
	}else{
		j=M-j*C;
		memset(c[i],'*',j);
		memset(c[i]+j,'.',C-j);
		i++;
		for(;i<R;i++)memset(c[i],'.',C);
	}
	c[R-1][C-1]='c';
	return 1;
}
int rec(int x,int y,int R,int C,int M){
	int v=0;
	if(M==0)return 1;
	if(y>=R)return 0;
	if(x>=C)return 0;
	if(x<0)return 0;
	if(y<0)return 0;
	if(x>1)if(c[y][x-1]=='.'&&(c[y][x-2]<4||c[y][x-2]=='*'))return 0;
	if(y>1)if(c[y-1][x]=='.'&&(c[y-2][x]<4||c[y-2][x]=='*'))return 0;
	if(x<C-2)if(c[y][x+1]=='.'&&(c[y][x+2]<4||c[y][x+2]=='*'))return 0;
	if(y<R-2)if(c[y+1][x]=='.'&&(c[y+2][x]<4||c[y+2][x]=='*'))return 0;
	if(c[y][x]=='*'||c[y][x]<1){
		if(c[y][x]>4)c[y][x]=0;
		else c[y][x]++;
		v|=rec(x-1,y,R,C,M);
		if(!v)v|=rec(x,y-1,R,C,M);
		if(!v)v|=rec(x+1,y,R,C,M);
		if(!v)v|=rec(x,y+1,R,C,M);
		if(c[y][x]==0)c[y][x]='*';
		else c[y][x]--;
		return v;
	}
	if(c[y][x]!='.')return 0;
	if(x-1==0&&c[y][x-1]=='.'){
		if(M<2)return 0;
		c[y][x]='*';
		v=rec(x-1,y,R,C,M-1);
		if(v)return 1;
		c[y][x]='.';
		return 0;
	}
	if(y-1==0&&c[y-1][x]=='.'){
		if(M<2)return 0;
		c[y][x]='*';
		v=rec(x,y-1,R,C,M-1);
		if(v)return 1;
		c[y][x]='.';
		return 0;
	}
	if(x+2==C){
		if(M<2)return 0;
		c[y][x]='*';
		v=rec(x+1,y,R,C,M-1);
		if(v)return 1;
		c[y][x]='.';
		return 0;
	}
	if(y+2==R){
		if(M<2)return 0;
		c[y][x]='*';
		v=rec(x,y+1,R,C,M-1);
		if(v)return 1;
		c[y][x]='.';
		return 0;
	}
	c[y][x]=0;
	v|=rec(x-1,y,R,C,M-1);
	if(!v)v|=rec(x,y-1,R,C,M-1);
	if(!v)v|=rec(x+1,y,R,C,M-1);
	if(!v)v|=rec(x,y+1,R,C,M-1);
	if(v)c[y][x]='*';
	else c[y][x]='.';
	return v;
}
int recs(int R,int C,int M){c[R-1][C-1]='c';return rec(0,0,R,C,M);}
int solve(int R,int C,int M){//handles output(atleast in some way)
	int i,j,v;
	if(R==1){
		for(i=1;i<=M;i++)putc('*',stdout);
		for(;i<C;i++)putc('.',stdout);
		puts("c");
		return -2;
	}
	if(C==1){
		for(i=1;i<=M;i++)puts("*");
		for(;i<R;i++)puts(".");
		puts("c");
		return -2;
	}
	if(M==0){
		for(i=1;i<R;i++){
			for(j=1;j<C;j++)putc('.',stdout);
			puts(".");
		}
		for(j=1;j<C;j++)putc('.',stdout);
		puts("c");
		return -2;
	}
	i=C*R-M;
	if(i==1){
		for(i=1;i<R;i++){
			for(j=1;j<C;j++)putc('*',stdout);
			puts("*");
		}
		for(j=2;j<C;j++)putc('*',stdout);
		puts("*c");
		return -2;
	}
	if((C==2||R==2)&&(M&1))goto noans;
	/*if(M==1){
		putc('*',stdout);
		for(j=2;j<C;j++)putc('.',stdout);
		puts(".");
		for(i=2;i<R;i++){
			for(j=1;j<C;j++)putc('.',stdout);
			puts(".");
		}
		for(j=2;j<C;j++)putc('.',stdout);
		puts(".c");
		return -2;
	}*/
	if(i<4)goto noans;
	if(i==5)goto noans;
	if(chkvvv(R,C,M))goto ans;
	//printf("%d\n",chkvvvr);
	if(chkvvv(C,R,M))goto rans;
	//printf("%d\n",chkvvvr);
	v=solve(R,C,M-1);
	if(v==1)return recs(R,C,1);
	if(v==2)return recs(C,R,1)==1?2:0;
	v=solve(R,C,M-2);
	if(v==1)return recs(R,C,2);
	if(v==2)return recs(C,R,2)==1?2:0;
	//if(v==0)return recs(R,C,M);
	return recs(R,C,M);
	fprintf(stderr,"NOANSWERSET %d %d %d\n",R,C,M);
	return -1;
noans:	return 0;
ans:	return 1;
rans:	return 2;
}
void out(int R,int C){int i;for(i=0;i<R;i++)puts(c[i]);}
void rout(int R,int C){
	int i,j;
	C--;
	for(i=0;i<R;i++){
		for(j=0;j<C;j++)putc(c[j][i],stdout);
		printf("%c\n",c[j][i]);
	}
}
int main(void){
	int T,i,R,C,M,v;
	scanf("%d",&T);
	for(i=1;i<=T;i++){
		memset(c,'.',10000);
		printf("Case #%d:\n",i);
		scanf("%d %d %d",&R,&C,&M);
		for(v=0;v<R;v++)c[v][C]=0;
		v=solve(R,C,M);
outp:		if(v==1)out(R,C);
		else if(v==2)rout(R,C);
		else if(v==0)puts("Impossible");
		else if(v==-1)puts("Unimplemented");
		else if(v<-10){
			v+=100;
			fprintf(stderr,"case %d %d %d %d\n",R,C,M,v);
			goto outp;
		}
	}
	return 0;
}

