/*
Coder: Butterfly21
Task: Watersheds
Source: Google Code Jam 2009 Qualification Round Problem B
*/
#include<cstdio>
#include<memory>
int grid[101][101],ans[101][101],op[100000],dx[]={-1,0,0,1},dy[]={0,-1,1,0},H,W,T,cnt,TT;
bool leg(int x,int y){return (x>=0)&&(x<H)&&(y>=0)&&(y<W);}
int trace(int x,int y,int v){
	//printf("%d %d %d\n",x,y,v);
	if(ans[x][y])return ans[x][y];
	int min=grid[x][y],go=-1;
	for(int i=0;i<4;i++)
	    if(leg(x+dx[i],y+dy[i])&&(grid[x+dx[i]][y+dy[i]]<min))min=grid[x+dx[i]][y+dy[i]],go=i;
	if(go==-1)
		return (ans[x][y]=cnt++);
	else if(ans[x+dx[go]][y+dy[go]])return (ans[x][y]=ans[x+dx[go]][y+dy[go]]);
	else{
        if(v==-1)ans[x][y]=cnt++;
		else ans[x][y]=v;
		return (ans[x][y]=trace(x+dx[go],y+dy[go],ans[x][y]));
	}

}
int main(){
	freopen("B-large.in","r",stdin);
	freopen("B-large.out","w",stdout);
	scanf("%d",&T);
	for(int i=0;i<T;i++){
		cnt=1,TT=1;
		memset(ans,0,sizeof(ans));
		memset(op,0,sizeof(op));
		scanf("%d%d",&H,&W);
		for(int j=0;j<H;j++)
			for(int k=0;k<W;k++)scanf("%d",&grid[j][k]);
		for(int j=0;j<H;j++)
		    for(int k=0;k<W;k++)
				if(!ans[j][k])trace(j,k,-1);
		printf("Case #%d:\n",i+1);
		for(int j=0;j<H;j++){
			if(!op[ans[j][0]])op[ans[j][0]]=TT++;
			putchar(op[ans[j][0]]+'a'-1);
		    for(int k=1;k<W;k++){
				if(!op[ans[j][k]])op[ans[j][k]]=TT++;
				printf(" %c",op[ans[j][k]]+'a'-1);
			}
		    putchar('\n');
		}
	}
	//scanf("  ");
}
