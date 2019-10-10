#include <vector>
#include <map>
#include <set>
#include <stack>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <string>
#include <queue>
#include <cmath>

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
#define FORE(it,X) for(__typeof((X).begin()) it=(X).begin(); it!=(X).end(); ++it)

VI dh,dw;

int main()
{
	int tn;

	cin>>tn;
	FOR(qq,1,tn+1) {
		int n;
		cin>>n;
		dh.clear();
		dw.clear();
		int nhl,nhr;
		int nwl,nwr;
		int hl,hr;
		int wl,wr;
		nhl=nwl=-234234234;
		nhr=nwr=234234234;
		hl=wl=234234234;
		hr=wr=-234234234;
		REP(i,n) {
			string a;
			int h,w;
			cin>>h>>w>>a;
			if (a=="BIRD") {
				hl=min(hl,h);
				wl=min(wl,w);
				hr=max(hr,h);
				wr=max(wr,w);
			}
			else if (a=="NOT") {
				cin>>a;
				dh.PB(h);
				dw.PB(w);
			}
		}
		int nobird=0;
		if (hl==234234234) nobird=1;

		cin>>n;
		printf("Case #%d:\n",qq);
		REP(i,n) {
			int h,w;
			cin>>h>>w;
			if (nobird) {
				int no=0;
				REP(j,SZ(dh))
					if (h==dh[j] && w==dw[j])
						no=1;
				if (no) cout<<"NOT BIRD"<<endl;
				else cout<<"UNKNOWN"<<endl;
				continue;
			}
			if (hl<=h && h<=hr && wl<=w && w<=wr)
				cout<<"BIRD"<<endl;
			else {
				int no=0;
				REP(j,SZ(dh)) {
					if (dh[j]<hl && dw[j]<wl) {
						if (h<=dh[j] && w<=dw[j])
							no=1;
					}
					else if (dh[j]<hl && dw[j]>wr) {
						if (h<=dh[j] && w>=dw[j])
							no=1;
					}
					else if (dh[j]>hr && dw[j]<wl) {
						if (h>=dh[j] && w<=dw[j])
							no=1;
					}
					else if (dh[j]>hr && dw[j]>wr) {
						if (h>=dh[j] && w>=dw[j])
							no=1;
					}
					else if (dh[j]<hl) {
						if (h<=dh[j])
							no=1;
					}
					else if (dh[j]>hr) {
						if (h>=dh[j])
							no=1;
					}
					else if (dw[j]<wl) {
						if (w<=dw[j])
							no=1;
					}
					else if (dw[j]>wr) {
						if (w>=dw[j])
							no=1;
					}
				}
				if (no)	cout<<"NOT BIRD"<<endl;
				else cout<<"UNKNOWN"<<endl;
			}
		}
	}
	return 0;
}
