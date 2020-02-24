#include<cstdio>
#include<algorithm>
#include<vector>
#include<string>
#include<cmath>
using namespace std;

const int INF=1000111222;

typedef long long ll;
typedef pair<int,int> pii;
typedef vector<int> vi;

struct Roslina {
	int x,y,r;
	void read() {
		scanf("%d%d%d",&x,&y,&r);
	}
};

Roslina t[41];

double sqr(double x) {
	return x*x;
}

double pokryj(const Roslina &a,const Roslina &b) {
	return (sqrt(sqr(a.x-b.x)+sqr(a.y-b.y))+a.r+b.r)*.5;
}

int main() {
	int testow;
	scanf("%d",&testow);
	for(int z=1;z<=testow;++z) {
		double wynik=0;
		int n;
		scanf("%d",&n);
		for(int i=0;i<n;++i) {
			t[i].read();
		}
		if(n==1) {
			wynik=t[0].r;
		} else if(n==2) {
			wynik=min(pokryj(t[0],t[1]),(double)max(t[0].r,t[1].r));
		} else if(n==3) {
			wynik=min(
					max((double)t[0].r,pokryj(t[1],t[2])),
				min(
					max((double)t[1].r,pokryj(t[0],t[2])),
					max((double)t[2].r,pokryj(t[0],t[1]))
				)
			);
		}
		printf("Case #%d: %.7f\n",z,wynik);
	}
}
