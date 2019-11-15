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

int ps_length(string str){

	for(int i = str.length()-1; i >= 1; i--){
		if(str.substr(0,i) == str.substr(str.length()-i,i)) return i;
	}

	return 0;
}

int main(){
	
	int T, K, L, S, max_poss; S(T);
	int tst = 1;
	double ans;

	int freq[26], freq2[26];
	bool flag;
	string keys, target;

	while(T--){

		ans = 0.0;
		flag = true;
		M(freq, 0);
		M(freq2, 0);
		S(K); S(L); S(S);
		cin>>keys>>target;

		F(i,0,K){
			freq[keys[i]-'A']++;
		}

		F(i,0,L){
			freq2[target[i]-'A']++;
			if(!freq[target[i]-'A']){
				flag = false;
				ans = 0.0;
				break;
			}
		}

		if(flag){
			max_poss = 1 + ((S - L)/(L - ps_length(target)));

			ans = (S-L+1);
			F(i,0,L){
				ans *= (float(freq[target[i]-'A'])/K);
			}
			ans = (max_poss - ans);
		}

		printf("Case #%d: %.9f\n", tst++, ans);
	}
}