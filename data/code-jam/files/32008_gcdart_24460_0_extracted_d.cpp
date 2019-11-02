#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <sstream>
#include <cstring>
#include <utility>
#include <algorithm>
#include <set>
#include <map>
#include <queue>
#include <deque>
#include <cassert>
#include <cmath>

using namespace std;


#define REP(i,N) for(int i = 0;i < (N); ++i ) 
#define REPV(i,ar) for(int i = 0; i < (ar).sz; ++i )
#define EACH(it,mp) for( __typeof(mp.begin()) it(mp.begin()); it != mp.end(); ++it )
#define FOR(i,a,b) for(int i = (a); i <= (b); ++i )
#define sz size()
#define pb push_back
#define cs cs_str()
#define mkp make_pair
#define INF (int(1e9))
#define LINF (LL(1e18))
#define sor(ar) sort((ar).begin(),(ar).end())
#define rev(ar) reverse((ar).begin(),(ar).end())
#define GI ({int t;scanf("%d",&t);t;})

typedef pair<int,int> PII;
typedef vector<int> VI;
typedef long long int LL;

char Map[64][64];
int R,C;
int Memo[1<<16][4][4];


#define ok(a,b) ((a) >= 0 && (b) >= 0 && (a) < R && (b) < C )

int GETBIT(int bit , int r , int c ) {
	int pos = r*4+c;
	return (bit&(1<<pos));
}

int SETBIT(int bit , int r , int c ) {
	int pos = r*4 + c;
	return (bit | (1<<pos) );
}

int isWin( int bit , int r , int c ) {
	int& ret = Memo[ bit ][ r ][ c ];
	if( ret != -1 ) return ret;
	assert( Map[r][c] != '#' );	
	ret = 0;
	
	FOR(dx,-1,1) FOR(dy,-1,1) if( dx || dy ) if( !(dx == 0 && dy == 0 ) ) {
		int x = r + dx, y = c + dy;
		if( ok(x,y) && Map[x][y] == '.' && GETBIT(bit,x,y) == 0 ) {
			int nbit = SETBIT(bit,x,y);
			int won = isWin( nbit , x , y );
			if( won == 0 ){ ret = 1;break;}
		}	
	}
	return ret;
}

int main(){
	int T = GI;
	FOR(tt,1,T){
		R = GI, C = GI;
		REP(i,R) scanf("%s",Map[i]);
		memset( Memo , -1 , sizeof( Memo ) );
		int ans = -1;
		REP(i,R) REP(j,C) if( Map[i][j] == 'K' ){
			int start = SETBIT(0,i,j);
			ans = isWin( start , i , j );
		}
		assert( ans != -1 );
		printf("Case #%d: %s\n",tt,ans?"A":"B");
	}
	return 0;
}
