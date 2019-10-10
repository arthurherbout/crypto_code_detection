#include <stdio.h>
#include <algorithm>
#include <vector>
#define INF (int)1000000
using namespace std;

long double a[32][4];
long double b[32][4];
long double ans;
int cur[32];
int n,m;
vector<long double> g;

int count() {
	int i,res=1;
	for (i=1;i<=n;i++) {		
		res*=cur[i];
		if (res>INF)
			res=INF;
	}
	return res;
}

void go(int i,long double s) {
	int j;
	if (i==n+1) {
		g.push_back(s);
		return;
	}
	for (j=0;j<cur[i];j++)
		go(i+1,s*a[i][j]);
}

int main () {
	int tests,w;
	int i,j,pos;
	freopen("in.txt","r",stdin);
	freopen("out.txt","w",stdout);
	for (scanf("%d",&tests),w=1;w<=tests;w++) {
		printf("Case #%d: ",w);
		scanf("%d%d",&m,&n);
		for (i=1;i<=n;i++) {
			for (j=0;j<4;j++)
				scanf("%Lf",&a[i][j]);
			sort(a[i],a[i]+4);
			reverse(a[i],a[i]+4);
			for (j=1;j<4;j++)
				if (a[i][j]>1e-8)
					b[i][j]=a[i][j]/a[i][j-1];
				else
					b[i][j]=0.0;
		}		
		for (i=1;i<=n;i++) 
			cur[i]=4;			/*
		while (count()<m) {
			pos=-1;
			for (i=1;i<=n;i++)
				if (cur[i]!=4 && (pos==-1 || b[pos][cur[pos]]<b[i][cur[i]]))
					pos=i;
			if (pos==-1) break;
			cur[pos]++;
		}				*/
		g.clear();
		go(1,1);
		sort(g.begin(),g.end());
		reverse(g.begin(),g.end());
		ans=0; 
		if ((int)g.size()<m)
			ans=1.0;
		else
			for (i=0;i<m;i++)
				ans+=g[i];
		printf("%Lf\n",ans);
	}
	return 0;
}
