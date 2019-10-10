#include <iostream>
#include <string>
#include <cstring>
#include <string.h>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <bitset>

using namespace std;

int n,m;
char c[200][200];
int used[10][10];

void solve(int tst){
	scanf("%d%d\n",&n,&m);
	for (int i=0; i<n; i++){
		for (int j=0; j<m; j++){
			scanf("%c",&c[i][j]);
		}
		scanf("\n");
	}

	int res=0;
	for (int i=0; i<(1<<(n*m)); i++){
		bitset<20> b=i;
		int ok=1;
		memset(used,0,sizeof(used));
		for (int j=0; j<n&&ok; j++)
			for (int k=0; k<m&&ok; k++)
				if (!used[j][k]){
					int curx=j;
					int cury=k;
					used[j][k]=1;
					while (1&&ok){
						int tp=b[curx*m+cury];
						if (tp==0) tp=-1;
						if (c[curx][cury]=='-') cury+=tp; else
						if (c[curx][cury]=='|') curx+=tp; else
						if (c[curx][cury]=='/') curx-=tp, cury+=tp; else
						if (c[curx][cury]=='\\')curx-=tp, cury-=tp;
						curx=(curx+n)%n;
						cury=(cury+m)%m;
						if (used[curx][cury]){
							if (!(curx==j&&cury==k))
								ok=0;
							break;
						}
						used[curx][cury]=1;
					}
				}
				res+=ok;
	}
	printf("Case #%d: %d\n",tst,res);
}

int main(){
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	int tests;
	scanf("%d\n",&tests);

	for (int tt=1; tt<=tests; tt++){
		solve(tt);
	}

	return 0;
}