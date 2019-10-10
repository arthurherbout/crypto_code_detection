#include <algorithm>
#include <cstdio>
#include <set>
#include <string>
#include <vector>

using namespace std;

typedef pair<int,int> par;

int n;
par a[1000];
int color[17];

set<par> memo;

int solve( int i, int top ) {
    if( i == n ) return 1;
    
    int broj = 0;
    for( int y = 1; y <= 15; ++y ) broj = broj * (top+1) + color[y];
    if( memo.count( par(i,broj)) ) return 0;
    memo.insert( par(i,broj) );    
    
    int prev = color[a[i].second];
    for( int c = 1; c <= top; ++c ) {
        int ok = 1;
        for( int y = a[i].second-1; y <= a[i].second+1; ++y ) 
            if( color[y] && color[y] == c ) 
                ok = 0;
        
        if( ok ) {
            color[a[i].second] = c;
            if( solve( i+1, top ) ) return 1;
            color[a[i].second] = prev;
        }
    }
    return 0;
}

int main( void ) {
    int T;
    scanf( "%d", &T );
    for( int tt = 1; tt <= T; ++tt ) {
        scanf( "%d", &n );
        for( int i = 0; i < n; ++i ) 
            scanf( "%d%d", &a[i].first, &a[i].second );
        
        sort( a, a+n );
        reverse( a, a+n ); 

        int top;
        for( top = 1; top <= 3; ++top ) {
            memset( color, 0, sizeof color );
            memo.clear();
            if( solve( 0, top ) ) break;
        }
        printf( "Case #%d: %d\n", tt, top );
    }
    return 0;
}
