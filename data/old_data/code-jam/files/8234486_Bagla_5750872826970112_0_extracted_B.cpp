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
#define pb(x) push_back(x)
#define mp make_pair
#define F(i, a, b) for (int i = int(a); i < int(b); i++)
#define forit(it, a) for (it = (a).begin(); it != (a).end(); it++)
#define M(x,i) memset(x,i,sizeof(x))

/* -------------------CODE GOES HERE---------------------- */

double eps = 0.000000001;

int main(){
	
	int T, N; S(T);
	double V, X, R1, C1, R2, C2;
	double x, y, ans;

	int tst = 1;
	bool poss;

	while(T--){


		ans = -1;
		S(N); SD(V); SD(X);

		if(N == 1){
			SD(R1); SD(C1);
			
			poss = false;
			if(fabs(X-C1) < eps) poss = true;
			if(poss) ans = (V/R1);
		}

		else if(N == 2){
			
			SD(R1); SD(C1); SD(R2); SD(C2);
			poss = true;

			if(C1 < C2){
				swap(C1, C2);
				swap(R1, R2);
			}

			if((C1 < X) || (C2 > X)) poss = false;
			
			if(fabs(C1 - C2) < eps){
				if(fabs(C1 - X) < eps) ans = V/(R1+R2);
				else poss = false;
			}

			if((ans == -1) && poss){

				y = (V*(C1 - X))/(R2 * (C1 - C2));
				x = (V*(X - C2))/(R1 * (C1 - C2));
				ans = max(x,y);
			}
		}

		if(!poss) printf("Case #%d: IMPOSSIBLE\n", tst++);
		else printf("Case #%d: %.9lf\n", tst++, ans);
	}	
}