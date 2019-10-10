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

	int T, tst = 1; S(T);
	int N;
	string t1,t2;
	// map<string, int> A,B;
	vector<string> A,B;
	vector<bool> c;
	int ans;

	while(T--){

		S(N);
		ans = 0;
		A.clear();
		// a.clear(); b.clear();
		c.clear();
		B.clear();

		F(i,0,N){
			cin>>t1>>t2;
			A.pb(t1);
			B.pb(t2);
			c.pb(false);
			// A[t1]++;
			// B[t2]++;
		}

		int n = A.size();

		bool flag = true;
		while(flag){

			flag = false;

			F(i,0,n){
				F(j,0,n){
					if(c[i] == true) continue;
					if(i == j) continue;
					if((A[i] == A[j]) && (c[j] == false)){
						F(k,0,n){
							if(i == k) continue;
							if((B[i] == B[k]) && (c[k] == false)){
								ans++;
								flag = true;
								c[i] = true;
							}
						}
					}
				}
			}
		}

		printf("Case #%d: %d\n", tst++, ans);
	}
}
