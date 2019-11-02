#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <bitset>
#include <set>
#include <map>
#include <vector>
#include <cmath>
#include <string>
#include <cstring>
#include <queue>
using namespace std;

#define FOR0(i,n) for( i = 0 ; i < n ; ++i )
#define FOR1(i,n) for( i = 1 ; i <= n ; ++i )
#define sys_p system( "pause" )
#define pb push_back 
#define mp make_pair

typedef long long LL ;
typedef vector < LL > vl ;
typedef vector < vl > vvl ;
typedef vector < vvl > vvvl ;

ifstream Cin( "input.txt" );
ofstream Cout( "output.txt" );

const LL MOD = 1000000007 ;

struct tribe
{
     LL d0, n, w0, e0, s0, dD, dP, dS ;
     void get() { Cin >> d0 >> n >> w0 >> e0 >> s0 >> dD >> dP >> dS ; }
};

struct attack
{
     LL day, l, r, s ;
     attack(){}
     attack(LL D,LL L,LL R,LL S){day=D,l=L,r=R,s=S;}  
};

bool operator < ( attack a, attack b ) 
{
     return a.day < b.day ;
}

main()
{ 
     LL T, t ; 
     Cin >> T ;
     FOR1( t, T )
     {
           LL N, i, j, old_wall[3001] = {}, new_wall[3001] = {}, badcur, bad = 0 ; tribe r ; vector < attack > a ;
           Cin >> N ;
           FOR0( i, N )
           {
                 r.get() ; r.w0 += 1000 ; r.e0 += 999 ;
                 a.pb( attack( r.d0, r.w0, r.e0, r.s0 ) ) ;
                 for( j = 2 ; j <= r.n ; j++ )
                   a.pb( attack( r.d0 + (j-1)*r.dD, r.w0 + (j-1)*r.dP, r.e0 + (j-1)*r.dP, r.s0 + (j-1)*r.dS ) ) ;
           }
           sort( a.begin(), a.end() ) ;
           FOR0( i, (LL)a.size() ) 
           {
                 //cout << bad << "   " << a[i].day << " " << a[i].l << " " << a[i].r << " " << a[i].s << endl ;
                 if( i && a[i].day > a[i-1].day ) FOR1( j, 3000 ) old_wall[j] = new_wall[j] ;
                 badcur = 0 ; 
                 for( j = a[i].l ; j <= a[i].r ; j++ )
                 {
                   if( old_wall[j] < a[i].s ) badcur++ ; 
                   new_wall[j] = max( new_wall[j], a[i].s ) ;
                 }
                 if( badcur ) bad++ ;
           }
           Cout << "Case #" << t << ": " << bad << endl ; 
     }
}
