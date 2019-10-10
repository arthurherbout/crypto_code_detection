#include<stdio.h>
#include<assert.h>
#include<string.h>
#include<math.h>
#include<assert.h>

#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<stack>
#include<queue>
#include<algorithm>
#include<utility>
using namespace std;

#define _abs(x)		(((x)>0)?(x):-(x))
#define _max(x,y)	(((x)>(y))?(x):(y))
#define _min(x,y)	(((x)<(y))?(x):(y))

#define EPS 1e-10
#define INF 1000000

#define S(x)	((x)*(x))
#define Z(x)	(_abs(x) < EPS)
#define N(x)	(x < 0 && !Z(x))
#define P(x)	(x > 0 && !Z(x))
#define ZN(x)	(x < 0 || Z(x))
#define ZP(x)	(x > 0 || Z(x))

#define UND		0
#define BIRD	1
#define NOB		2

#define E(x,y)	(Z((x)-(y)))

#define D2(a,b)	(S(a.x-b.x) + S(a.y-b.y))
#define D1(a,b)	(sqrt(D2(a,b)))

#define T2(a,b,c)	((a.x*b.y+b.x*c.y+c.x*a.y) - (a.y*b.x+b.y*c.x+c.y*a.x))

typedef __int64 LL;

int m;
double p;
int x;

double dp[16][1000005];

double get(int k,int y){
	int i,lo,hi,m1,m2;
	double p1,p2,ppp;

	LL yy = y;
	for(i=0;i<k;i++)
		yy *=2;

	if(yy < INF || y<=0)
		return 0;

	if(y >= INF)
		return 1;

	if(k==1){
		if(2*y < INF)	return 0;
		else			return p;
	}

	if(dp[k][y] != -1)
		return dp[k][y];

	lo = 0;
	hi = y;
	ppp = 0;
	
	while(hi-lo > 5){
		m1 = (lo*2+hi)/3;
		m2 = (lo+hi*2)/3;
		
		p1 = p*get(k-1,y+m1) + (1-p)*get(k-1,y-m1);
		p2 = p*get(k-1,y+m2) + (1-p)*get(k-1,y-m2);

//		printf(">>> %d %d %.15lf %.15lf\n",lo,hi,p1,p2);

//		if(p1 > p2 && !( Z(p1) && Z(p2) ) ){
		if(p1 > p2 && !Z(p1-p2) ){
			hi = m2+1;
			ppp = p1;
		}
		else{
//			printf("**\n");
			lo = m1-1;
			ppp = p2;
		}
	}
	for(m1=lo;m1<=hi;m1++){
		p1 = p*get(k-1,y+m1) + (1-p)*get(k-1,y-m1);
		if(ppp < p1)		ppp=p1;
	}

	dp[k][y] = ppp;
	return dp[k][y];
}

int main(){

	int T,N;

	int i,j,k;

	scanf("%d",&T);
	for(N=1;N<=T;N++){

		scanf("%d%lf%d",&m,&p,&x);

		for(i=0;i<=m;i++)
			for(j=0;j<=INF;j++)
				dp[i][j] = -1;


		printf("Case #%d: %.6lf\n",N , get(m,x) );




	}
	return 0;
}



