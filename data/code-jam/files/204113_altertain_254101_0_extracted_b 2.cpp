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

struct ele {
	int i,j,k,l;
	int c;

	ele() {}
	ele(int _i,int _j,int _k,int _l,int _c)
	{
		i=_i;
		j=_j;
		k=_k;
		l=_l;
		c=_c;
	}

	bool operator<(ele t) const
	{
		return c>t.c; // min heap
	}
};

int n,m,f;
char dt[16][9];
int dy[16][9][1<<8][1<<8];
priority_queue<ele> pq;

int basic(int y)
{
	int c=0;

	REP(i,m) if (dt[y][i]=='#')
		c^=1<<i;
	return c;
}

bool AND(int j,int i)
{
	return (j&(1<<i))>0;
}

int XOR(int j,int i)
{
	return j^(1<<i);
}

void PUSH(int i,int j,int k,int l,int c)
{
	if (dy[i][j][k][l] > c) {
		pq.push(ele(i,j,k,l,c));
		dy[i][j][k][l]=c;
	}
}

bool fall(int &y,int &x)
{
	int t=1;
	
	while (dt[y+1][x]=='.') {
		y++;
		t++;
	}
	return t<=f;
}

int main()
{
	int tn;

	cin>>tn;
	FOR(qq,1,tn+1) {
		cin>>n>>m>>f;
		REP(i,n) cin>>dt[i];
		REP(i,m) dt[n][i]='#';

		memset(dy,63,sizeof(dy));

//		REP(i,n+1) cout<<basic(i)<<" "; cout<<endl;

		while( !pq.empty() ) pq.pop();
		PUSH(0,0,basic(0),basic(1),0);

		int dp=234234234;
		while (!pq.empty()) {
			int i,j,k,l,c;

			i=pq.top().i;
			j=pq.top().j;
			k=pq.top().k;
			l=pq.top().l;
			c=pq.top().c; pq.pop();

//			printf("%d %d %d %d %d\n",i,j,k,l,c);
//			char qqq[5]; gets(qqq);

			if (dy[i][j][k][l] != c) continue;
			if (i==n-1) {
				dp=c;
				break;
			}

			if (j>0) { // j-1 °¡´É
				if ( AND(k,j-1) == false && AND(l,j-1) == true ) {
					PUSH(i,j-1,k,l,c);
					PUSH(i,j,k,XOR(l,j-1),c+1);
				}

				if ( AND(k,j-1) == false && AND(l,j-1) == false ) { // ¶³¾îÁü
					int y,x;
					y=i+1, x=j-1;
//					cout<<"y : "<<y<<", x : "<<x<<endl;	cout<<"fall : "<<fall(y,x)<<endl; cout<<"y : "<<y<<", x : "<<x<<endl;
					y=i+1, x=j-1;
					if (fall(y,x)) {
						if (y==i+1) { // ÇÑÄ­ ¶³¾îÁü
							PUSH(y,x,l,basic(y+1),c);
						}
						else { // µÎÄ­ ÀÌ»ó
							PUSH(y,x,basic(y),basic(y+1),c);
						}
					}
				}
			}

			if (j<m-1) { // j+1 °¡´É
//				cout<<AND(k,j+1)<<", "<<AND(l,j+1)<<endl;

				if ( AND(k,j+1) == false && AND(l,j+1) == true ) {
					PUSH(i,j+1,k,l,c);
					PUSH(i,j,k,XOR(l,j+1),c+1);
				}

				if ( AND(k,j+1) == false && AND(l,j+1) == false ) { // ¶³¾îÁü
					int y,x;
					y=i+1, x=j+1;
//					cout<<"y : "<<y<<", x : "<<x<<endl; cout<<"fall : "<<fall(y,x)<<endl; cout<<"y : "<<y<<", x : "<<x<<endl;
					y=i+1, x=j+1;
					if (fall(y,x)) {
						if (y==i+1) { // ÇÑÄ­ ¶³¾îÁü
							PUSH(y,x,l,basic(y+1),c);
						}
						else { // µÎÄ­ ÀÌ»ó
							PUSH(y,x,basic(y),basic(y+1),c);
						}
					}
				}
			}
		}

		printf("Case #%d: ",qq);
		if (dp==234234234) cout<<"No"<<endl;
		else cout<<"Yes "<<dp<<endl;
	}
	return 0;
}
