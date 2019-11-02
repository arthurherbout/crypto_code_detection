#include <stdio.h>
#include <memory.h>
int move[4][2] = { {1,1}, {1,0}, {0,1}, {1,-1} };
  //  \      -      |      /
int Type[5][5],Mark[5][5];
int main()
{
	int T;scanf("%d",&T);
	for (int kase=1;kase<=T;++kase)
	{
		int N,M;
		char str[256];
		scanf("%d %d",&N,&M);
		for (int r=0;r<N;++r)
		{
			scanf("%s",str);
			for (int q=0;q<M;++q) switch(str[q])
			{
				case '-': Type[r][q] = 1; break;
				case '|': Type[r][q] = 2; break;
				case '/': Type[r][q] = 3; break;
				default:  Type[r][q] = 0;
			}
		}
		int ret=0;
		for (int q=0;q<1<<(N*M);++q)
		{
			bool fail = false;
			memset(Mark,0,sizeof(Mark));
			for (int w=0;w<N;++w) for (int e=0;e<M;++e)
			{
				int rev = 1&(q>>(w*M+e));
				int ww = w + move[ Type[w][e] ][1] * (rev ? -1: 1);
				int ee = e + move[ Type[w][e] ][0] * (rev ? -1: 1);				
				ww=(ww+N)%N;
				ee=(ee+M)%M;
				if (Mark[ww][ee]) fail = true;
				Mark[ww][ee]=1;//(w*M+e)*2;
			}
			if (!fail) ret++;
		}
		printf("Case #%d: %d\n",kase,ret%1000003);
	}
	return 0;
}
