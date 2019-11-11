#include <vector>
#include <map>
#include <set>
#include <stack>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <string>
#include <cctype>
#include <cstring>
#include <queue>
#include <cassert>

using namespace std;

typedef long long LL;
typedef long double LD;
typedef vector<int> VI;
typedef pair<int,int> PII;
typedef vector<PII> VPII;

#define MP make_pair
#define ST first
#define ND second
#define PB push_back
#define FOR(i,a,b) for( int i=(a); i<(b); ++i)
#define FORD(i,a,b) for( int i=(a); i>(b); --i)
#define REP(i,n) for(int i=0; i<(n); ++i)
#define ALL(X) (X).begin(),(X).end()
#define SZ(X) (int)(X).size()
#define FORE(it,X) for(__typeof((X).begin()) it=(X).begin(); it!=(X).end();++it)

string dt[128];
vector<string> hubo;
int n,m;
string ans,str;

bool same(string &a, string &b, char c)
{
	REP(i,SZ(a)) {
		if (a[i]==c && b[i]!=c)
			return false;
		if (a[i]!=c && b[i]==c)
			return false;
	}
	return true;
}

bool contain(string& a,char b)
{
	return count(ALL(a),b);
}

int doit()
{
	int c=SZ(hubo);
	int hcnt=c;
	VI no(c,0);
	int go=0;
	int point =0;
	
	while (1) {
		if (hcnt==1) return point;
		
		bool gothis=false;
		REP(i,c) if (!no[i]) {
			if (contain(hubo[i],str[go])) {
				gothis=true;
				break;
			}
		}
		if (!gothis) {
			go++;
			continue;
		}

		if (!contain(ans,str[go]))
			point--;

		REP(i,SZ(hubo)) if (!no[i]) {
			if (!same(ans,hubo[i],str[go])) {
				hcnt--;
				no[i]=true;
			}
		}
		go++;
	}
	return point;
}

int main()
{
	int tn;
	cin>>tn;

	while (tn--) {
		cin>>n>>m;
		REP(i,n) cin>>dt[i];
		
		static int qq=0;
		printf("Case #%d:",++qq);
		while (m--) {
			cin>>str;
			int dp=1;
			string prt;
			REP(i,n) {
				hubo.clear();
				REP(j,n) if (SZ(dt[i])==SZ(dt[j]))
					hubo.PB(dt[j]);
				ans=dt[i];
				int now=doit();
//				cout<<endl<<ans<<" "<<now;

				if (dp>now) {
					dp=now;
					prt=ans;
				}
			}
//			cout<<endl;

			cout<<" "<<prt;
		}
		cout<<endl;
	}
	return 0;
}
