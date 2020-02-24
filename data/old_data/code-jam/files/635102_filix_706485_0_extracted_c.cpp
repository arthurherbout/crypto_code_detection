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

const int MX=205;

bool t[MX][MX],p[MX][MX];

void inline jeden() {
	for(int i=0;i<MX;++i)
		for(int j=0;j<MX;++j)
			t[i][j]=false;
	int r;
	scanf("%d",&r);
	for(int k=0;k<r;++k) {
		int x1,x2,y1,y2;
		scanf("%d%d%d%d",&x1,&y1,&x2,&y2);
		for(int i=x1;i<=x2;++i)
			for(int j=y1;j<=y2;++j) {
				t[i][j]=true;
			}
	}
	int wynik=0;
	for(;;++wynik) {
		/*for(int i=0;i<MX;++i) {
			for(int j=0;j<MX;++j) {
				if(t[i][j])
					printf("1");
				else
					printf("0");
			}
			puts("");
		}
		puts("");//*/
		bool koniec=true;
		for(int i=0;i<MX;++i)
			for(int j=0;j<MX;++j) {
				if(t[i][j]) {
					koniec=false;
					goto out;
				}
				p[i][j]=false;
			}
		if(koniec)
			break;
		out:;
		for(int i=1;i<MX;++i)
			for(int j=1;j<MX;++j)
				if(t[i-1][j]&&t[i][j-1])
					p[i][j]=true;
				else if(!t[i-1][j] && !t[i][j-1])
					p[i][j]=false;
				else
					p[i][j]=t[i][j];
		for(int i=0;i<MX;++i)
			for(int j=0;j<MX;++j) {
				t[i][j]=p[i][j];
			}
	}
	printf("%d\n",wynik);
}

int main() {
	int testow;
	scanf("%d",&testow);
	for(int z=1;z<=testow;++z) {
		printf("Case #%d: ",z);
		jeden();
	}
}
