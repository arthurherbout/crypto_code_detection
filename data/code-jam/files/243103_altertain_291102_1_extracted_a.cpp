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
#define Y first
#define X second
#define PB push_back
#define FOR(i,a,b) for( int i=(a); i<(b); ++i)
#define FORD(i,a,b) for( int i=(a); i>(b); --i)
#define REP(i,n) for(int i=0; i<(n); ++i)
#define ALL(X) (X).begin(),(X).end()
#define SZ(X) (int)(X).size()
#define FORE(it,X) for(__typeof((X).begin()) it=(X).begin(); it!=(X).end();++it)

//int dy[12][12][12][12];
map<VPII,int> dy;
char dt[16][16];
int n,m;
queue<VPII> q;
int hy[4]={0,0,-1,1};
int hx[4]={1,-1,0,0};

void PUSH(VPII v,int c)
{
	sort(ALL(v));
	REP(i,SZ(v)) {
		if (v[i].Y<0 || v[i].Y>=n) return;
		if (v[i].X<0 || v[i].X>=m) return;
	}

	if (dy.find(v)!=dy.end()) return;
	
	dy[v]=c; //v[0].Y][v[0].X][v[1].Y][v[1].X] = c;
//	cout<<v[0].Y<<" "<<v[0].X<<" "<<v[1].Y<<" "<<v[1].X<<" : "<<c<<endl;
//	if (chk) v.pop_back();
	q.push(v);
}

bool stable(VPII& v)
{
	queue<PII> q;
	VI chk(SZ(v),0);

	q.push(v[0]);
	chk[0]=1;

	while (!q.empty()) {
		PII t=q.front(); q.pop();

		REP(i,SZ(v)) if (!chk[i]) {
			PII u=v[i];
			if (abs(t.Y-u.Y)+abs(t.X-u.X)==1) {
				q.push(v[i]);
				chk[i]=1;
			}
		}
	}

	return count(ALL(chk),1)==SZ(v);
}

bool isin(PII a)
{
	int y=a.Y;
	int x=a.X;

	return (0<=y && y<n && 0<=x && x<m);
}

int main()
{
	int tn;

	cin>>tn;
	FOR(qq,1,tn+1) {
		cin>>n>>m;
		REP(i,n) cin>>dt[i];

		VPII v;
		REP(i,n) REP(j,m) if (dt[i][j]=='o' || dt[i][j]=='w')
			v.PB(MP(i,j));

		while (!q.empty()) q.pop();
		dy.clear();
//		memset(dy,63,sizeof(dy));
		PUSH(v,0);

		PII p[4];
		VPII dp;
		bool end;
		while (!q.empty()) {
			VPII v=q.front(); q.pop();
			int c=dy[v];
//			if (SZ(v)==2) c=dy[v[0].Y][v[0].X][v[1].Y][v[1].X];
//			else c=dy[v[0].Y][v[0].X][0][0];
			
			bool psta = stable(v),sta;

//	cout<<v[0].Y<<" "<<v[0].X<<" "<<v[1].Y<<" "<<v[1].X<<endl;

			end=true;
			REP(i,SZ(v)) if (dt[v[i].Y][v[i].X]!='x' && dt[v[i].Y][v[i].X]!='w')
				end=false;

			if (end) {
				dp=v;
				break;
			}

			REP(i,SZ(v)) {
				REP(j,4) p[j]=MP(v[i].Y+hy[j],v[i].X+hx[j]);
				PII back=v[i];

				if (isin(p[0]) && isin(p[1])) {
//					cout<<"here0"<<endl;
//					cout<<p[0].Y<<" "<<p[0].X<<endl;
//					cout<<p[1].Y<<" "<<p[1].X<<endl;
					if (dt[p[0].Y][p[0].X]!='#' && dt[p[1].Y][p[1].X]!='#') {
	//					cout<<"here1"<<endl;
						if (count(ALL(v),p[0])==0 && count(ALL(v),p[1])==0) {
							v[i]=p[0];
							sta=stable(v);
							if (psta || sta) PUSH(v,c+1);

							v[i]=p[1];
							sta=stable(v);
							if (psta || sta) PUSH(v,c+1);
						}
					}
				}

				v[i]=back;

				if (isin(p[2]) && isin(p[3]))
					if (dt[p[2].Y][p[2].X]!='#' && dt[p[3].Y][p[3].X]!='#') {
	//					cout<<"here2"<<endl;
						if (count(ALL(v),p[2])==0 && count(ALL(v),p[3])==0) {
							v[i]=p[2];
							sta=stable(v);
							if (psta || sta) PUSH(v,c+1);

							v[i]=p[3];
							sta=stable(v);
							if (psta || sta) PUSH(v,c+1);
						}
					}

				v[i]=back;
			}
		}

		printf("Case #%d: ",qq);
		if (end) {
			int c=dy[dp];
/*			if (SZ(dp)==2)
				dy[dp[0].Y][dp[0].X][dp[1].Y][dp[1].X];
			else
				dy[dp[0].Y][dp[0].X][0][0];*/
			cout<<c<<endl;
		}
		else cout<<"-1"<<endl;

	}
	return 0;
}
