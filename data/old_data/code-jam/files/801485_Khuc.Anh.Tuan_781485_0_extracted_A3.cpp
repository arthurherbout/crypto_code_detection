#include <iostream>
#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>
#include <map>
#include <set>
#include <queue>
using namespace std;

#define Rep(i,n) for(int i=0;i<n;++i)
#define For(i,a,b) for(int i=(a);i<=(b);++i)
#define Ford(i,a,b) for(int i=(a);i>=(b);--i)
#define fi first
#define se second
#define MP make_pair
#define pb push_back

typedef vector<int> VI;
typedef pair<int,int> PII;

char a[7070];
int n;
int inf = 1000000000;

int b[6][3];

int F[7070][7070][6];

int ds[7070];
int nd;

int G[7070];

int main() {
	
	{
		VI v;
		Rep(i,3)v.pb(i);
		int nb=0;
		do{
			Rep(i,3)b[nb][i]=v[i];
			++nb;
		}while(next_permutation(v.begin(),v.end()));
	}
	
	int ntest;
	scanf("%d", &ntest);
	gets(a);

	For(test,1,ntest) {
		cerr << "test = " << test << endl;
		gets(a);
		n = strlen(a);
		

		Rep(k,6) {
			Rep(st,n) {
				int step = 0;
				nd = 0;
				For(i,st,n-1) {
					if(nd > 0 && ds[nd-1] == a[i] - 'A') {
						// khong can thay doi
						++step;
						F[st][i][k] = step + nd;
					}
					else if(nd == 0) {
						// them 
						while(nd == 0 || ds[nd-1] != a[i] - 'A') {
							ds[nd] = b[k][nd%3];
							++nd;
							++step;
						}
						++step;
						F[st][i][k] = step + nd;
					}
					else {
						if(nd >= 2 && ds[nd-2] == a[i] - 'A') {
							++step;
							--nd;
	
							++step;
							F[st][i][k] = step + nd;
						}
						else {
							while(nd == 0 || ds[nd-1] != a[i] - 'A') {
								ds[nd] = b[k][nd%3];
								++nd;
								++step;
							}
							++step;
							F[st][i][k] = step + nd;
						}
					}
				}
				
			}
		}
		
		memset( G, 0x1f, sizeof(G));
		G[0] = 0;
		
		for(int i=1;i<=n;++i) {
			for(int j=0;j<i;++j) {
				Rep(k,6) 
					G[i] <?= G[j] + F[j][i-1][k];
			}
		}
		
		int res = G[n];
		
		printf("Case #%d: ", test);
		printf("%d\n", res);
		fflush(stdout);
	}
    return 0;
}
