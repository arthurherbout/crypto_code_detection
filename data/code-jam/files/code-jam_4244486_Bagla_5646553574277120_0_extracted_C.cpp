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

bool orig[31], cop[31];

int check(int V){

	F(i,1,V+1){
		if(!cop[i]) return i;
	}

	return -1;
}

int main(){
	
	int T, C, D, V, temp; S(T);
	int tst = 1, ans;

	while(T--){

		M(orig, false);
		M(cop, false);
		ans = 0;

		S(C); S(D); S(V);
		
		F(i,0,D){
			S(temp);
			orig[temp] = true;
			cop[temp] = true;
		}

		while(check(V) != -1){

			F(i,0,V+1){
				cop[i] = orig[i];		
			}

			for(int i = V; i >= 1; i--){
				if(cop[i]){
					for(int j = i-1; j >= 1; j--){
						if(cop[j] && ((i+j) <= V)) cop[i+j] = true;
					}
				}
			}

			if(check(V) != -1){
				orig[check(V)] = true;
				ans++;
			}
		}

		printf("Case #%d: %d\n", tst++, ans);
	}
}