// ayy
// ' lamo
#include <bits/stdc++.h>
#include <bits/extc++.h>
using namespace std;
using namespace __gnu_pbds;
typedef long long ll;
typedef long double ld; //CARE
typedef complex<ld> pt;
#define fi first
#define se second
#define pb push_back
const ld eps=1e-8;
const int inf=1e9+5000;
const ll linf=1e18+99;
const int P=1e9+7;

const int N=256;
int r,c,n,d,req[N][N];
ll z[N][N];






void _m(int tn) {
	scanf("%d%d%d%d",&r,&c,&n,&d);
	for(int i=1;i<=r;i++) for(int j=1;j<=c;j++) req[i][j]=-1, z[i][j]=linf;
	for(int _i=0;_i<n;_i++) {
		int ri,ci,bi; scanf("%d%d%d",&ri,&ci,&bi);
		req[ri][ci]=bi;
		for(int i=1;i<=r;i++) for(int j=1;j<=c;j++) {
			ll _d=abs(i-ri)+abs(j-ci);
			_d *= d;
			z[i][j]=min(z[i][j],_d+bi);
		}
	}
	ll ss;
	for(int i=1;i<=r;i++) for(int j=1;j<=c;j++) if(~req[i][j]) {
		assert(z[i][j]<=req[i][j]);
		if(z[i][j]!=req[i][j]) goto die;
	}

	ss=0;
	for(int i=1;i<=r;i++) for(int j=1;j<=c;j++) ss+=z[i][j]%P;
	printf("Case #%d: %d\n",tn,int(ss%P));
	return;
	die:; printf("Case #%d: IMPOSSIBLE\n",tn);
}


int32_t main() {
	int T; scanf("%d",&T); for(int tn=1;tn<=T;tn++) _m(tn);
}










