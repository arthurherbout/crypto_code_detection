#define DEBUG 1

#include <algorithm>
#include <functional>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <complex>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cassert>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <sstream>
using namespace std;

#define LL long long
#define LD long double
#define PR pair<int,int>

#define Fox(i,n) for (i=0; i<n; i++)
#define Fox1(i,n) for (i=1; i<=n; i++)
#define FoxI(i,a,b) for (i=a; i<=b; i++)
#define FoxR(i,n) for (i=(n)-1; i>=0; i--)
#define FoxR1(i,n) for (i=n; i>0; i--)
#define FoxRI(i,a,b) for (i=b; i>=a; i--)
#define Foxen(i,s) for (i=s.begin(); i!=s.end(); i++)
#define Min(a,b) a=min(a,b)
#define Max(a,b) a=max(a,b)
#define Sz(s) int((s).size())
#define All(s) (s).begin(),(s).end()
#define Fill(s,v) memset(s,v,sizeof(s))
#define pb push_back
#define mp make_pair
#define x first
#define y second

template<typename T> T Abs(T x) { return(x<0 ? -x : x); }
template<typename T> T Sqr(T x) { return(x*x); }
string plural(string s) { return(Sz(s) && s[Sz(s)-1]=='x' ? s+"en" : s+"s"); }

const int INF = (int)1e9;
const LD EPS = 1e-12;
const LD PI = acos(-1.0);

//#if DEBUG
#define GETCHAR getchar
/*#else
#define GETCHAR getchar_unlocked
#endif*/

bool Read(int &x)
{
	char c,r=0,n=0;
	x=0;
		for(;;)
		{
			c=GETCHAR();
				if ((c<0) && (!r))
					return(0);
				if ((c=='-') && (!r))
					n=1;
				else
				if ((c>='0') && (c<='9'))
					x=x*10+c-'0',r=1;
				else
				if (r)
					break;
		}
		if (n)
			x=-x;
	return(1);
}

int N,M,K,A,B;
int ind[30][30];
int Ay[10],Ax[10],By[10],Bx[10];
char G[30][31];
bool can[10][1<<10][10];
vector<PR> lst;
int dyn[1<<10][1<<10];
int prvB1[1<<10][1<<10],prvB2[1<<10][1<<10];
PR prvP[1<<10][1<<10];

int main()
{
		if (DEBUG)
			freopen("in.txt","r",stdin);
	freopen("D-small-attempt2.in","r",stdin);
	//freopen("B-large.in","r",stdin);
	freopen("out.txt","w",stdout);
	int T,t;
	int i,j,k,a,b,d,y,x,z,b1,b2;
	queue<PR> Q;
	int dist[30][30];
	int my[4]={-1,1,0,0};
	int mx[4]={0,0,-1,1};
	bool W[30][30];
	Read(T);
		Fox1(t,T)
		{
			Read(M),Read(N),Read(K);
			A=B=0;
				Fox(i,N)
				{
					scanf("%s",&G[i]);
						Fox(j,M)
							if (G[i][j]=='S')
								ind[i][j]=A,Ay[A]=i,Ax[A++]=j;
							else
							if (G[i][j]=='T')
								ind[i][j]=B,By[B]=i,Bx[B++]=j;
				}
			Fill(can,0);
				Fox(a,A)
					Fox(b,1<<B)
					{
						Fill(W,0);
							Fox(i,N)
								Fox(j,M)
									if (G[i][j]=='#')
										W[i][j]=1;
									else
									if (G[i][j]=='T')
									{
										k=ind[i][j];
											if (!(b&(1<<k)))
												W[i][j]=1;
									}
						Fill(dist,-1);
						Q.push(mp(Ay[a],Ax[a])),dist[Ay[a]][Ax[a]]=0;
							while (!Q.empty())
							{
								i=Q.front().x;
								j=Q.front().y;
								Q.pop();
								d=dist[i][j];
								bool tur=0;
									FoxI(k,i+1,N-1)
										if (G[k][j]=='#')
											break;
										else
										if ((G[k][j]=='T') && (W[k][j]))
											can[a][b][ind[k][j]]=1,tur=1;
									FoxRI(k,0,i-1)
										if (G[k][j]=='#')
											break;
										else
										if ((G[k][j]=='T') && (W[k][j]))
											can[a][b][ind[k][j]]=1,tur=1;
									FoxI(k,j+1,M-1)
										if (G[i][k]=='#')
											break;
										else
										if ((G[i][k]=='T') && (W[i][k]))
											can[a][b][ind[i][k]]=1,tur=1;
									FoxRI(k,0,j-1)
										if (G[i][k]=='#')
											break;
										else
										if ((G[i][k]=='T') && (W[i][k]))
											can[a][b][ind[i][k]]=1,tur=1;
									if (tur)
										continue;
								d++;
									if (d>K)
										continue;
									Fox(z,4)
									{
										y=i+my[z];
										x=j+mx[z];
											if ((y<0) || (y>=N) || (x<0) || (x>=M) || (W[y][x]) || (dist[y][x]>=0))
												continue;
										Q.push(mp(y,x)),dist[y][x]=d;
									}
							}
					}
			Fill(dyn,-1);
			dyn[0][0]=0;
			lst.clear();
				Fox(b1,1<<A)
					Fox(b2,1<<B)
					{
						d=dyn[b1][b2]+1;
							if (!d)
								continue;
							Fox(i,A)
								if (!(b1&(1<<i)))
									Fox(j,B)
										if (!(b2&(1<<j)))
											if (can[i][b2][j])
											{
												int X=b1|(1<<i);
												int Y=b2|(1<<j);
													if (d<=dyn[X][Y])
														continue;
												dyn[X][Y]=d;
												prvB1[X][Y]=b1;
												prvB2[X][Y]=b2;
												prvP[X][Y]=mp(i+1,j+1);
											}
					}
				Fox(b1,1<<A)
					Fox(b2,1<<B)
						if (dyn[b1][b2]>Sz(lst))
						{
							lst.resize(dyn[b1][b2]);
							int X=b1;
							int Y=b2;
							k=dyn[b1][b2];
								while (X+Y)
								{
									lst[--k]=prvP[X][Y];
									i=prvB1[X][Y];
									j=prvB2[X][Y];
									X=i;
									Y=j;
								}
						}
			printf("Case #%d: %d\n",t,Sz(lst));
				Fox(i,Sz(lst))
					printf("%d %d\n",lst[i].x,lst[i].y);
		}
	return(0);
}