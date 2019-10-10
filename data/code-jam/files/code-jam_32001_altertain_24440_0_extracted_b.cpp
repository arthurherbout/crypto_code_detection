#include <cstdio>
#include <algorithm>
#include <vector>
#include <iostream>
#include <string>

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
#define FOR(i,a,b) for( i=(a); i<(b); ++i)
#define FORD(i,a,b) for( int i=(a); i>(b); --i)
#define REP(i,n) for(i=0; i<(n); ++i)
#define ALL(X) (X).begin(),(X).end()
#define SZ(X) (int)(X).size()
#define FORE(it,X) for(__typeof((X).begin()) it=(X).begin(); it!=(X).end(); ++it)

void ee(int a,int b,int &x,int &y,int &d)
{
	int dd,xx,yy;

	if (b==0) { d=a,x=1,y=0; return; }
	ee(abs(b),abs(a)%abs(b),xx,yy,dd);
//	cout<<"dd yy xx : "<<dd<<" "<<yy<<" "<<xx<<endl;

	d=dd;
	x=yy;
	y=xx-abs(a/b)*yy;

	if (a<0) x=-x;
	if (b<0) y=-y;
//	cout<<"d y x : "<<d<<" "<<y<<" "<<x<<endl;
}

int main()
{
	int tn,qq=0;

/*	int x,y,d;
	ee(0,3,x,y,d);
	cout<<x<<" "<<y<<endl;*/

	cin>>tn;
	while (tn--) {
		int n,m,A;

		cin>>n>>m>>A;
		int x1,x2,x3,y1,y2,y3;
		int end=0;
		printf("Case #%d: ",++qq);
		x1=y1=0;
		REP(x2,n+1) {
		REP(x3,n+1) {
		REP(y2,m+1) {
		REP(y3,m+1) {
			int ww=x1*y2-x2*y1+x2*y3-x3*y2+x3*y1-x1*y3;
			ww=abs(ww);
			if (ww==A) {
				end=1;
				break;
			}
		}
			if (end) break;
		}
			if (end) break;
		}
			if (end) break;
		}
//		if (n*m<A) {
		if (!end) cout<<"IMPOSSIBLE"<<endl;
		else
			printf("%d %d %d %d %d %d\n",x1,y1,x2,y2,x3,y3);
		
		continue;

		REP(x1,n+1) {
		FOR(x2,x1,n+1) {
		REP(y2,m+1) {
		FOR(y1,y2,m+1) {
			int C=A-x1*y2+x2*y1;
			int a,b,x0,y0,g;
			a=x2-x1;
			b=y1-y2;
			if (a==0 && b==0) continue;

//			printf("%d %d %d %d\n",x1,y1,x2,y2);
//			cout<<"a,b : "<<a<<","<<b<<endl;

			swap(a,b);
			ee(a,b,y0,x0,g);
//			cout<<"good"<<endl;

			if (C%g) continue;

//			printf("%d %d %d %d\n",x1,y1,x2,y2);

			x0=x0*(C/g);
			y0=y0*(C/g);
			
			int l1,r1,l2,r2;
			if (a==0) {
				l1=234234234;
				r1=234234234;
			}
			else {
				l1=(y0-m)*g/a;
				r1=y0*g/a;
			}

			if (b==0)
				l2=r2=234234234;
			else {
				l2=(-x0)*g/b;
				r2=(n-x0)*g/b;
			}
		
			l1=max(l1,l2);
			r1=min(r1,r2);
			if (l1>r1) continue;
			int e=l1;

			x3=x0+b*e/g;
			y3=y0-a*e/g;
			end=1;
			break;
		}
		if (end) break;
		}
		if (end) break;
		}
		if (end) break;
		}

		printf("Case #%d: ",++qq);

		if (!end) cout<<"IMPOSSIBLE"<<endl;
		else
/*		int ww=x1*y2-x2*y1+x2*y3-x3*y2+x3*y1-x1*y3;
		
		ww=abs(ww);
		if (ww!=A) cout<<"error!"<<endl;*/
			printf("%d %d %d %d %d %d\n",x1,y1,x2,y2,x3,y3);
//		}
	}
	return 0;
}
