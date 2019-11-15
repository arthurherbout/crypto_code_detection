#include <iostream>
#include <string>

using namespace std;

int dx[] = {0, 1, 1, 1, 0, -1, -1, -1};
int dy[] = {1, 1, 0, -1, -1, -1, 0, 1};

int main(){
	int TEST; cin >> TEST;
	string st[4];
	int dir[4][4];
	for(int test=1;test<=TEST;test++){
		int R, C; cin >> R >> C;
		for(int i=0;i<R;i++) cin >> st[i];
		for(int i=0;i<R;i++){
			for(int j=0;j<C;j++){
				if(st[i][j]=='-') dir[i][j] = 0;
				else if(st[i][j]=='|') dir[i][j] = 2;
				else if(st[i][j]=='/') dir[i][j] = 3;
				else dir[i][j] = 1;
			}
		}
		int res = 0;
		for(int i=0;i<(1<<(R*C));i++){
			bool visit[4][4];
			bool ok = true;
			memset(visit, false, sizeof(visit));
			for(int j=0;j<R;j++){
				for(int k=0;k<C;k++){
					int d = 4*((i>>(j*C+k))&1)+dir[j][k];
					int nx = (j+dx[d]+R)%R, ny = (k+dy[d]+C)%C;
					if(visit[nx][ny]) ok = false;
					visit[nx][ny] = true;
				}
			}
			if(ok) res++;
		}
		printf("Case #%d: %d\n", test, res);
	}
}
