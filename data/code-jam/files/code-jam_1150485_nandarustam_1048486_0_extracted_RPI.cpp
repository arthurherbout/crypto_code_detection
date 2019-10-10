#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <queue>
#include <complex>
#include <cmath>
#include <map>
using namespace std;

#define REP(i,n) for(int i = 0; i < n; ++i)
#define FOR(i,a,b) for(int i = a; i <= b; ++i)
#define FORd(i,a,b) for(int i = a; i >= b; --i)
#define mset(ar,x) memset(ar,x,sizeof(ar))
#define pub push_back
#define fr first
#define sc second
#define mp make_pair

typedef long long llong;
int tc,n;
char tabel[105][105];
double wp[105],owp[105],oowp[105];

int main()
{
	scanf("%d",&tc);
	FOR(t,1,tc){
		scanf("%d",&n);
		FOR(i,1,n) FOR(j,1,n) scanf("\n%c",&tabel[i][j]);
		
		//compute wp
		FOR(i,1,n){
			int kount = 0, match = 0;
			FOR(j,1,n) if(tabel[i][j] != '.'){
				++match;
				if(tabel[i][j] == '1') ++kount;
			}
			wp[i] = (double)kount / (double)match;
		}
		
		//compute owp
		FOR(i,1,n){
			double kount = 0;
			int match = 0;
			FOR(j,1,n) if(tabel[i][j] != '.'){
				int k = 0, m = 0;
				FOR(l,1,n) if(tabel[j][l] != '.' && l != i){
					++m;
					if(tabel[j][l] == '1') ++k;
				}
				++match;
				kount += (double)k / (double)m;
			}
			owp[i] = kount / (double)match;
		}
		
		//compute oowp
		FOR(i,1,n){
			double kount = 0;
			int match = 0;
			FOR(j,1,n) if(tabel[i][j] != '.'){
				++match;
				kount += owp[j];
			}
			oowp[i] = kount / (double)match;
		}
		
		printf("Case #%d:\n",t);
		FOR(i,1,n){
			double ans = 0.25 * wp[i] + 0.50 * owp[i] + 0.25 * oowp[i];
			char lho[100];
			sprintf(lho,"%.12lf",ans);
			int ln = strlen(lho)-1;
			while(lho[ln] == '0') lho[ln] = '\0', --ln;
			printf("%s\n",lho);
		}
	}
	
	return 0;
}


