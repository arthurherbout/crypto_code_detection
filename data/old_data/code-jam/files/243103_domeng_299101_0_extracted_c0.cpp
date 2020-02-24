#include <stdio.h>
#include <algorithm>
#include <map>
#include <vector>
using namespace std;
typedef pair<int,int> pii;
typedef vector<pii> piivec;
int N;
piivec vec;
int soloTest()
{
		for (int q=0;q<N;++q) for (int w=q+1;w<N;++w)
				if ( abs(vec[q].second - vec[w].second) <=1 )
						return 0;
		return 1;
}
const int NMAX=100;
const int RMAX=16;
int Dp[NMAX][1<<RMAX];
int Ex[RMAX];
int coupleTest()
{
		for (int q=0;q<RMAX;++q) Ex[q]=0;
		for (int q=0;q<N;++q) for (int w=0;w<(1<<RMAX);++w) Dp[q][w]=0;
		Dp[0][0]=1;
		Ex[vec[0].second]=1;
		for (int q=0;q<N;++q) 
		{
				for (int w=0;w<(1<<RMAX);++w)
				if (Dp[q][w])
				{
						if (q==N-1) return 1;
						int r0=vec[q+1].second;
						for (int myC=0;myC<=1;++myC)
						{
								int err=0;
								for (int r=r0-1;r<=r0+1;++r)
								{
										if (r<0 || r>=RMAX) continue;
										if (!Ex[r]) continue;
										if ( ((w>>r)&1) == myC) err=1;
								}
								if (err) continue;
								int next = w;
								next &= ~(1<<r0);
								next |= (myC<<r0);
								Dp[q+1][next]=1;
						}
				}
				if (q+1<N) Ex[vec[q+1].second]=1;
		}
		return 0;
}
int main()
{
		int T;
		scanf("%d",&T);
		for (int kase=1;kase<=T;++kase)
		{
				scanf("%d",&N);
				vec.clear();
				for (int q=0;q<N;++q)
				{
						pii tmp;
						scanf("%d %d",&tmp.first,&tmp.second);
						vec.push_back(tmp);
				}
				sort(vec.rbegin(),vec.rend());
				printf("Case #%d: ",kase);
				if ( soloTest() ) printf("1\n");
				else if ( coupleTest()) printf("2\n");
				else printf("3\n");
		}
		return 0;
}
