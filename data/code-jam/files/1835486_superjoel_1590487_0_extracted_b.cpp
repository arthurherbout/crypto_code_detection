#include <stdio.h>
#include <map>
#include <string>
#include <queue>
#include <set>
using namespace std;

int S, M;
int g[7000][7000];
int h[7000][7000];
int mark;

int dx[6] = {0, 1, 1, 0,-1,-1};
int dy[6] = {1, 1, 0,-1,-1, 0};

int getEgenskap(int x, int y){
	if(x==1 && y==1)return 1<<1;
	if(x==1 && y==S)return 1<<2;
	if(x==S && y==2*S-1)return 1<<3;
	if(x==2*S-1 && y==2*S-1)return 1<<4;
	if(x==2*S-1 && y==S)return 1<<5;
	if(x==S && y==1)return 1<<6;

	if(x==1)return 1<<7;
	if(y-x == S-1)return 1<<8;
	if(y==2*S-1)return 1<<9;
	if(x==2*S-1)return 1<<10;
	if(x-1 == S-1)return 1<<11;
	if(y==1)return 1<<12;

	return 0;
}

int inne(int x, int y){
	if(x<1)return 0;
	if(y-x>S-1)return 0;
	if(y>2*S-1)return 0;
	if(x>2*S-1)return 0;
	if(x-y>S-1)return 0;
	if(y<1)return 0;
	return 1;
}

void floodFill(int sx, int sy, int egenskap){
	queue<pair<int, int> > q;
	q.push(make_pair(sx, sy));
	while(!q.empty()){
		pair<int, int> nu = q.front();
		q.pop();
		int nux = nu.first;
		int nuy = nu.second;
		for(int dir=0;dir<6;dir++){
			int nyx = nux+dx[dir];
			int nyy = nuy+dy[dir];
			if(!inne(nyx, nyy))continue;
			if(g[nyy][nyx] != -1){
				if(g[nyy][nyx] != egenskap){
					q.push(make_pair(nyx, nyy));
				}
				g[nyy][nyx] = egenskap;
			}
		}
		g[nuy][nux] = egenskap;
	}
}

int vinnBridge(int e){
	int bryggor = 0;
	for(int i=1;i<=6;i++){
		if(e & (1<<i))bryggor++;
	}
	return bryggor>=2;
}

int vinnFork(int e){
	int forks = 0;
	for(int i=7;i<=12;i++){
		if(e & (1<<i))forks++;
	}
	return forks>=3;
}

int enableRingDebug=0;
int vinnRing(int x, int y){
	if(enableRingDebug)printf("\nvinnRing(%d,%d)\n",x,y);
	set<pair<int, int> > apa;
	for(int d=0;d<6;d++){
		int nux = x+dx[d];
		int nuy = y+dy[d];		
		if(inne(nux, nuy) && g[nuy][nux] == -1){
			while(inne(nux, nuy) && g[nuy][nux] == -1){
				int nextx = nux + dx[d];
				int nexty = nuy + dy[d];
				nux=nextx;
				nuy=nexty;
			}
			if(inne(nux, nuy)){
				apa.insert(make_pair(nux, nuy));
				if(enableRingDebug)printf("apa (%d,%d)\n", nux, nuy);
			}
		}
	}
	if(apa.empty())return 0;
	for(int startdir=0;startdir<6;startdir++){
		int okvinst = 0;
		mark++;
		if(mark<0){
			memset(h, 0, sizeof(h));
			mark = 1;
		}
		int omkrets=1;
		h[y][x] = mark;
		int nux = x+dx[startdir];
		int nuy = y+dy[startdir];
		int lastdir = startdir;
		if(inne(nux, nuy) && g[nuy][nux] != -1){
			int klar = 0;
			while(!klar){
				omkrets++;
				if(enableRingDebug)printf("startdir %d, omkrets %d (%d,%d)\n", startdir, omkrets, nux,nuy);
				h[nuy][nux] = mark;
				int firstdir = (lastdir+2)%6;
				for(int ddir=0;ddir>-6;ddir--){
					int dir = (firstdir+ddir+12)%6;
					if(enableRingDebug)printf("try dir %d\n", dir);
					int nextx = nux + dx[dir];
					int nexty = nuy + dy[dir];
					if(nextx == x && nexty==y){
						if(enableRingDebug)printf("omkrets blev %d\n", omkrets);
						klar = 1;
						if(okvinst && omkrets >= 6){
							return 1;
						}
						break;
					}
					if(inne(nextx, nexty) && g[nexty][nextx] != -1){
						if(enableRingDebug)printf("ddir %d, dir %d, look at (%d,%d)\n", ddir, dir, nextx, nexty);
						if(h[nexty][nextx] == mark){
							klar = 1;
							break;
						}
						nux = nextx;
						nuy = nexty;
						if(apa.find(make_pair(nux, nuy)) != apa.end()){
							okvinst = 1;
						}
						lastdir = dir;
						break;
					}
				}
			}
		}
	}
	return 0;
}

int vinnRing2(int x, int y){
	if(enableRingDebug)printf("\nvinnRing(%d,%d)\n",x,y);
	for(int startdir=0;startdir<6;startdir++){
		mark++;
		if(mark<0){
			memset(h, 0, sizeof(h));
			mark = 1;
		}
		int omkrets=1;
		h[y][x] = mark;
		int nux = x+dx[startdir];
		int nuy = y+dy[startdir];
		int lastdir = startdir;
		if(inne(nux, nuy) && g[nuy][nux] != -1){
			int klar = 0;
			while(!klar){
				omkrets++;
				if(enableRingDebug)printf("startdir %d, omkrets %d (%d,%d)\n", startdir, omkrets, nux,nuy);
				h[nuy][nux] = mark;
				int firstdir = (lastdir+4)%6;
				for(int ddir=0;ddir<6;ddir++){
					int dir = (firstdir+ddir+12)%6;
					if(enableRingDebug)printf("try dir %d\n", dir);
					int nextx = nux + dx[dir];
					int nexty = nuy + dy[dir];
					if(nextx == x && nexty==y){
						if(enableRingDebug)printf("omkrets blev %d\n", omkrets);
						klar = 1;
						if(omkrets >= 6){
							return 1;
						}
						break;
					}
					if(inne(nextx, nexty) && g[nexty][nextx] != -1){
						if(enableRingDebug)printf("ddir %d, dir %d, look at (%d,%d)\n", ddir, dir, nextx, nexty);
						if(h[nexty][nextx] == mark){
							klar = 1;
							break;
						}
						nux = nextx;
						nuy = nexty;
						lastdir = dir;
						break;
					}
				}
			}
		}
	}
	return 0;
}

void run(int fall){
	//enableRingDebug = fall==4;
	//enableRingDebug = 1;
	printf("Case #%d: ", fall);
	scanf("%d %d", &S, &M);
	int vunnen = 0;
	memset(g, -1, sizeof(g));
	for(int drag=0;drag<M;drag++){
		int dragx, dragy;
		scanf("%d %d", &dragx, &dragy);
		if(!vunnen){
			int myEgen = getEgenskap(dragx, dragy);
			int my = myEgen;
			for(int dir=0;dir<6;dir++){
				int nyx = dragx+dx[dir];
				int nyy = dragy+dy[dir];
				if(!inne(nyx, nyy))continue;
				if(g[nyy][nyx] != -1){
					my |= g[nyy][nyx];
				}
			}
			g[dragy][dragx] = my;
			floodFill(dragx, dragy, my);

			int vinner = 0;
			if(vinnBridge(my)){
				vinner = 1;
				printf("bridge");
			}
			if(vinnFork(my)){
				if(vinner)printf("-");
				vinner = 1;
				printf("fork");
			}
			if(vinnRing(dragx, dragy)){
				if(vinner)printf("-");
				vinner = 1;
				printf("ring");
			}
			if(vinner){
				printf(" in move %d\n", drag+1);
				vunnen = 1;
			}
		}
	}
	if(!vunnen){
		printf("none\n");
	}
}


int main(){
	int T;
	scanf("%d", &T);
	for(int i=0;i<T;i++){
		run(i+1);
	}	
}
