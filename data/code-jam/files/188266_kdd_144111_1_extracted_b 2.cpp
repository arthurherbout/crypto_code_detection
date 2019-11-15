#include <iostream>
#include <cstdio>
#include <set>
using namespace std;
int light[100][100][5];
int q[1000000][5];
int best[30][30][5];
int n,m;
int west_alt[10]={0,2,1,4,3};
int north_alt[10]={0,4,3,2,1};
int dx[10]={0,0,-1,0,1};
int dy[10]={0,1,0,-1,0};
bool valid(int x,int y) {
	return (x>=1 && x<=n && y>=1 && y<=m);
}

bool iscross(int loc,int dir) {
	if (loc==1 && (dir==1 || dir==2)) return true;
	if (loc==2 && (dir==2 || dir==3)) return true;
	if (loc==3 && (dir==3 || dir==4)) return true;
	if (loc==4 && (dir==4 || dir==1)) return true;
	return false;
}

void reach(int x,int y,int loc,int dir,int &destx,int &desty,int &destloc) {
	if (dir==1 || dir==3) destloc=west_alt[loc];
	else destloc=north_alt[loc];

	if (iscross(loc,dir)) {
		destx=x;
		desty=y;
		
	}
	else {
		destx=dx[dir]+x;
		desty=dy[dir]+y;
	}
}
int waittime(int x,int y,int dir,int t) {
	int tt=t-light[x][y][3];
	if (tt<0) tt+=light[x][y][1]+light[x][y][2];
	tt%=(light[x][y][1]+light[x][y][2]);
	int w1,w2;
	if (tt<light[x][y][1]) {
		w1=0;
		w2=light[x][y][1]-tt;
	}
	else {
		w1=light[x][y][1]+light[x][y][2]-tt;
		w2=0;
	}
	if (dir==1 || dir==3) return w2;
	return w1;
}

		
	

int main() {
	freopen("B-large.in","r",stdin);
	freopen("B-large.out","w",stdout);
	int tn;
	scanf("%d",&tn);
	
	for (int ti=1;ti<=tn;++ti) {
		scanf("%d %d",&n,&m);
		for (int i=1;i<=n;++i)
			for (int j=1;j<=m;++j) {
				scanf(" %d %d %d ",light[i][j]+1,light[i][j]+2,light[i][j]+3);
				light[i][j][3]%=(light[i][j][1]+light[i][j][2]);
			}
		int head=1,rear=1;
		q[1][1]=n;
		q[1][2]=1;
		q[1][3]=1;
		q[1][4]=0;
		memset(best,0xff,sizeof(best));
		best[n][1][1]=0;
		while (head>=rear) {
			int x=q[rear][1];
			int y=q[rear][2];
			int loc=q[rear][3];
			int t=q[rear][4];
			if (t>best[x][y][loc]) {
				rear++;
				continue;
			}
			int destx,desty,destloc,destt;
			for (int dir=1;dir<=4;++dir) {
				reach(x,y,loc,dir,destx,desty,destloc);
				if (!valid(destx,desty)) continue;
				
				if (!iscross(loc,dir)) destt=t+2;
				else destt=t+waittime(x,y,dir,t)+1;
				if (best[destx][desty][destloc]==-1 || best[destx][desty][destloc]>destt) {
					best[destx][desty][destloc]=destt;
					head++;
					q[head][1]=destx;
					q[head][2]=desty;
					q[head][3]=destloc;
					q[head][4]=destt;
				}
				
			}
			
			
			rear++;
			
		}
		cout<<"Case #"<<ti<<": "<<best[1][m][3]<<endl;
	}
	return 0;
}
