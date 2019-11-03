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

int n,m;
int dt[16][1001];

int main()
{
	int tn;

	cin>>tn;
	FOR(qq,1,tn+1) {
		memset(dt,0,sizeof(dt));
		cin>>n;
//		cout<<"n : "<<n<<endl;
		set<int> xset;
		REP(i,n) {
			int x,y;
			cin>>x>>y;
			x--,y--;
			xset.insert(x);
			dt[y][x]=-1;
		}

		n=15;
		m=1000;
		int c=1;
		int dp=1;
		REP(i,m) if (dt[0][i]==-1) {
			dt[0][i]=c;
			dp=max(dp,c);
			if (c==1) c=2;
			else c=1;
		}

		FOR(i,1,n) {
			REP(j,m) if (dt[i][j]==-1) {
				int k;
				VI c(6,0);
				bool flag=false;
				for (k=j-1;k>=0;k--) {
					if (dt[i][k]!=0) {
						c[dt[i][k]]=1;
						break;
					}
					if (!flag && dt[i-1][k]!=0) {
						flag=true;
						c[dt[i-1][k]]=1;
					}
				}

				flag=false;
				for (k=j+1;k<m;k++) {
					if (dt[i][k]!=0) {
						c[dt[i][k]]=1;
						break;
					}
					if (!flag && dt[i-1][k]!=0) {
						flag=true;
						c[dt[i-1][k]]=1;
					}
				}
				for (k=1;k<6;k++) if (c[k]==0) {
					dt[i][j]=k;
					break;
				}
				dp=max(dp,k);
			}
		}
/*
		REP(i,n) {
			REP(j,m) if (xset.find(j)!=xset.end())
				cout<<dt[i][j];
			cout<<endl;
		}
		cout<<endl;
*/
		int four=0;
		REP(i,n) REP(j,m) if (dt[i][j]==4)
			four++;

		printf("Case #%d: ",qq);
		if (four && four<=10) dp--;
		cout<<dp<<endl;
	}
	return 0;
}
