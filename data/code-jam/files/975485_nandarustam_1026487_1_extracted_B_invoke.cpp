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

typedef long long llong;
typedef complex<double> point;
typedef pair<char,char> Pcc;
int tc,c,d,n;
vector<char> v;
int kount[200];
map<Pcc,char> mep;
map<char,char> oposit;
char tmps[200];
string s;

int main()
{
	scanf("%d",&tc);
	FOR(t,1,tc){
		mep.clear();
		v.clear();
		oposit.clear();
		mset(kount,0);
		scanf("%d",&c);
		REP(i,c){
			scanf("%s",tmps);
			mep[mp(tmps[0],tmps[1])] = tmps[2];
			mep[mp(tmps[1],tmps[0])] = tmps[2];
		}
		scanf("%d",&d);
		REP(i,d){
			scanf("%s",tmps);
			oposit[tmps[0]] = tmps[1];
			oposit[tmps[1]] = tmps[0];
		}
		scanf("%d",&n);
		scanf("%s",tmps); s = tmps;
		
		REP(i,n){
			if(v.size() == 0){
				v.pub(s[i]);
				++kount[s[i]];
			}else if(mep.count(mp(s[i],v.back())) > 0){
				char si = s[i];
				char bak = v.back();
				while(mep.count(mp(si,bak)) > 0){
					--kount[bak];
					v.pop_back();
					si = mep[mp(si,bak)];
					if(v.size() == 0) break;
					bak = v.back();
				}
				if(oposit.count(si) > 0 && kount[oposit[s[i]]] > 0){
					mset(kount,0);
					v.clear();
				}else{
					v.pub(si);
					++kount[si];
				}
			}else if(oposit.count(s[i]) > 0 && kount[oposit[s[i]]] > 0){
				mset(kount,0);
				v.clear();
			}else{
				v.pub(s[i]);
				++kount[s[i]];
			}
		}
		printf("Case #%d: [",t);
		if(v.size() > 0){
			printf("%c",v[0]);
			FOR(i,1,v.size()-1) printf(", %c",v[i]);
		}
		printf("]\n");
	}
}

