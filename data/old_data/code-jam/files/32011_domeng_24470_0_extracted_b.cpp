#include <string>
#include <vector>
#include <cstdio>
#include <cmath>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <queue>
using namespace std;
int Check[100][100];
int BN(int x) { return 1<<x; }
int N,M;
int good(int y,int x){ return y>=0 && x>=0 && y<N && x<M; }
int main()
{
	srand((unsigned)time(NULL));
	freopen("B-small-attempt0.in","r",stdin);
	freopen("B-small.out","w",stdout);
	int kase,T,q;
	int Dir[2][2];
	scanf("%d",&T);
	for (kase=1;kase<=T;kase++)
	{
		scanf("%d %d",&N,&M);
		for (q=0;q<2;q++) scanf("%d %d",Dir[q],Dir[q]+1);
		int y,x,ret=1;
		scanf("%d %d",&y,&x);
		queue<int>Q;
		Q.push(y*M+x);
		Check[y][x]=kase;
		while (!Q.empty())
		{
			y=Q.front()/M, x=Q.front()%M;
			Q.pop();
			for (q=0;q<2;q++) if (good(y+Dir[q][0],x+Dir[q][1])) 
				if (Check[y+Dir[q][0]][x+Dir[q][1]]<kase)
				{
					ret++;
					Check[y+Dir[q][0]][x+Dir[q][1]]=kase;
					Q.push((y+Dir[q][0])*M+x+Dir[q][1]);
				}
		}
		printf("Case #%d: %d\n",kase,ret);
	}
	return 0;
}