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
#define bpc __builtin_popcount
 
typedef long long ll;
const int mod = 10007;
int n;
int dat[1024];
int main() {
	int e = 0, T;
	freopen("B.in","r",stdin);
	scanf("%d",&T);
	while(T--) {
		scanf("%d",&n);
		FOR(i,0,n) scanf("%d",&dat[i]);
		bool unknown = false;
		int ans = -1;
		if (n==1 || n==2 || n==3) unknown = true;
		else if (n%2 == 1){
			for(int i=1;i+3<n;i+=2) {
				int a = dat[i], b = dat[i+1], c = dat[i+2], d = dat[i+3];
				if((b-a+mod)%mod == (d-c+mod)%mod) {
					unknown = true;
					break;
				}
			}
			if(!unknown) {
				ans = (dat[n-1] + (dat[1]-dat[0]) + mod)%mod;
			}
		}
		else {
			for(int i=0;i+3<n;i+=2) {
				int a = dat[i], b = dat[i+1], c = dat[i+2], d = dat[i+3];
				if((b-a+mod)%mod == (d-c+mod)%mod) {
					unknown = true;
					break;
				}
			}
			if(!unknown) {
				ans = (dat[n-1] + (dat[2]-dat[1]) + mod)%mod;
			}
		}
		if(unknown)
			printf("Case #%d: UNKNOWN\n",++e);
		else {
			printf("Case #%d: %d\n",++e,ans);
		}
	}
	return 0;
}

