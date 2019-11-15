#include<cstdio>
#include<algorithm>
#include<vector>
#include<list>
#include<stack>
#include<queue>
#include<cmath>
#include<map>
#include<string>
#include<set>
#include<cstring>
#include<iostream>
#include<sstream>
using namespace std;
 
#define PB push_back
#define FORE(i,t) for(typeof(t.begin())i=t.begin();i!=t.end();++i)
#define SZ(x) int((x).size())

typedef long long ll;
typedef vector<int> vi;
typedef pair<int,int> pii;

const int INF=1000111222;

int t[105][105];

int p[205][205];

bool ustaw(int &a,int &b) {
	if(a==-1)
		a=b;
	else if(b==-1)
		b=a;
	return a==b;
}

void inline jeden() {
	int k;
	scanf("%d",&k);
	for(int i=0;i<2*k-1;++i) {
		for(int j=0;j<(i<k?i+1:2*k-1-i);++j)
			scanf("%d",t[i]+j);
	}
	for(int wynik=k;;++wynik) {
		//printf("k=%d wynik=%d\n",k,wynik);
		int tox=(wynik-k)*2+1;
		for(int x=0;x<tox;++x) {
			int toy=(x*2<tox?x+1:tox-x);
			for(int y=0;y<toy;++y) {
				//printf("x=%d / %d y=%d / %d\n",x,tox,y,toy);
				for(int i=0;i<2*wynik-1;++i) {
					int len=(i<wynik?i+1:2*wynik-1-i);
					for(int j=0;j<len;++j) {
						p[i][j]=-1;
						//printf("x ");
					}
					//puts("");
				}
				for(int i=0;i<2*k-1;++i) {
					int len=(i<k?i+1:2*k-1-i);
					for(int j=0;j<len;++j)
						p[i+x][j+y]=t[i][j];
				}
				bool ok=true;
				for(int i=0;i<2*wynik-1;++i) {
					int len=(i<wynik?i+1:2*wynik-1-i);
					for(int j=0;j<len;++j) {
					//printf("i=%d j=%d\n",i,j);
					//printf("%d %d %d %d\n",p[i][j],p[2*wynik-2-i][j],p[i][j],p[i][len-j-1]);
						if(!ustaw(p[i][j],p[2*wynik-2-i][j]) || !ustaw(p[i][j],p[i][len-j-1])) {
							
							ok=false;
							goto out;
						}
					}
				}
				if(ok) {
					printf("%d\n",wynik*wynik-k*k);
					return;
				}
				out:;
			}
		}
		
		
	}
}

int main() {
	int testow;
	scanf("%d",&testow);
	for(int z=1;z<=testow;++z) {
		printf("Case #%d: ",z);
		jeden();
	}
}
