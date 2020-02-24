#include <map>     
#include <set>     
#include <cmath>    
#include <cstdio>   
#include <vector>     
#include <string>     
#include <sstream>    
#include <iostream>    
#include <algorithm>     
using namespace std;     
#define FOR(i,a,b) for(int i=(a); i<(b); ++i)     
#define FORE(it,x) for(typeof(x.begin()) it=x.begin(); it!=x.end(); ++it)     
#define SET(x, v) memset(x, v, sizeof (x))     
#define sz size()     
#define cs c_str()     
#define pb push_back     
#define mp make_pair    
 
typedef long long i64;     

int R, C;
char dat[64][64];
int main() {

	freopen("E.in","r",stdin);
	int e = 0, T;
	scanf("%d",&T);
	while(T--) {
		scanf("%d%d",&R,&C);
		FOR(i,0,R) {
			scanf("%s",dat[i]);
		}
		int ans = 0;
		FOR(i,0,R)
			FOR(j,0,C) {
				int cnt = 0;
				if(i && dat[i-1][j]=='#') cnt++;
				if(i+1<R && dat[i+1][j]=='#') cnt++;
				if(j && dat[i][j-1]=='#') cnt++;
				if(j+1<C && dat[i][j+1]=='#') cnt++;
				if(dat[i][j]=='#') {
					ans+= (4 - cnt);
				}
				else if(dat[i][j]=='?') {
					if(cnt==0) {
						dat[i][j] = '#';
						ans+=4;
					}
					if(cnt==1) {
						dat[i][j] = '#';
						ans+= 2;
					}
				}
			}
		ans = 0;
		FOR(i,0,R){
			//printf("%s\n",dat[i]);
			FOR(j,0,C) 
				if(dat[i][j]=='#') {
					int cnt = 0;
					if(i && dat[i-1][j]=='#') cnt++;
					if(i+1<R && dat[i+1][j]=='#') cnt++;
					if(j && dat[i][j-1]=='#') cnt++;
					if(j+1<C && dat[i][j+1]=='#') cnt++;
					ans += (4-cnt);
				}
		}

		printf("Case #%d: %d\n",++e, ans);
	}


	return 0;
}




