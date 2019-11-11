#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<vector>
#include<algorithm>
#include<bitset>
#include<list>
#include<set>
#include<map>
#include<stack>
#include<queue>
#include<cmath>
#include<string>
#include<cstring>
#include<sstream>
#include<climits>

using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef set<int> si;
typedef map<string, int> msi;

#define S(x) scanf("%d",&x)
#define SD(x) scanf("%lf",&x)
#define SL(x) scanf("%lld",&x)
#define pb(x) push_back(x)
#define mp make_pair
#define F(i, a, b) for (int i = int(a); i < int(b); i++)
#define forit(it, a) for (it = (a).begin(); it != (a).end(); it++)
#define M(x,i) memset(x,i,sizeof(x))

/* -------------------CODE GOES HERE---------------------- */

int main(){
	
	int T; S(T);
	int tst = 1;

	int N, H;
	int fi, ft, si, st, ans;

	while(T--){

		ans = 0;
		S(N);

		if(N == 1){
			S(fi);
			S(H);
			S(ft);

			si = fi;
			st = ft + 1;
		}

		else if(N == 2){

			S(fi);
			S(H);
			S(ft);

			S(si);
			S(H);
			S(st);

			if(ft > st){
				swap(ft, st);
				swap(fi, si);
			}
		}
		
		if((float((360 - si)*st)/(360)) >= (float((720 - fi)*ft)/(360))) ans = 1;

		printf("Case #%d: %d\n", tst++, ans);
	}
}