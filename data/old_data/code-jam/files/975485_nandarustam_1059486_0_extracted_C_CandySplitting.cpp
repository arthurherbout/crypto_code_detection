#include <cstdio>
#include <algorithm>
#include <cstring>
#include <complex>
#include <cmath>
#include <vector>
#include <map>
#include <set>
using namespace std;

#define REP(i,n) for(int i = 0; i < n; ++i)
#define FOR(i,a,b) for(int i = a; i <= b; ++i)
#define FORd(i,a,b) for(int i = a; i >= b; --i)
#define mset(ar,x) memset(ar,x,sizeof(ar))
#define x real()
#define y imag()
#define mp make_pair
#define pub push_back
#define fr first
#define sc second

int tc,n,bil[20];

int main()
{
	scanf("%d",&tc);
	FOR(t,1,tc){
		scanf("%d",&n);
		REP(i,n) scanf("%d",&bil[i]);
		int ans = -1;
		FOR(bit,1,(1 << n)-2){
			int sum1 = 0, sum2 = 0;
			int jml1 = 0, jml2 = 0;
			REP(i,n){
				if(bit & (1 << i)){
					sum1 = (sum1^bil[i]);
					jml1 += bil[i];
				}else{
					sum2 = (sum2^bil[i]);
					jml2 += bil[i];
				}
			}
			if(sum1 == sum2) ans = max(ans,max(jml1,jml2));
		}
		if(ans == -1)printf("Case #%d: NO\n",t);
		else printf("Case #%d: %d\n",t,ans);
	}
	
	return 0;
}

