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
#include <string>
#include <cstring>
#include <queue>
#include <memory.h>
#include <cmath>
using namespace std;

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define FOR0(i,n) for( i = 0 ; i < n ; ++i )
#define FOR1(i,n) for( i = 1 ; i <= n ; ++i )
#define sys_p system( "pause" )
#define pb push_back
#define mp make_pair
#define FI first
#define SE second
#define sz(a) (int)a.size()

typedef long long LL ;

int n, i, j, m, ans, cnt ;
string s ; char c ;
vector < string > r[300] ;
set < string > all ;
set < string > en, fr ;

map < string, int > code ;
vector < int > ir[300] ;
set < int > ien, ifr, sameb, newen, newfr, same ;

set < int > allr ;
vector < vector < int > > alln ;
vector < int > whon ;

set < string > :: iterator it ;

int main() {
    //ifstream Cin("input.txt");
    freopen("output.txt", "w", stdout);

    int _T, _NT ;
    cin >> _T ;
    for( _NT = 1 ; _NT <= _T ; ++_NT ) {
        cout << "Case #" << _NT << ": ";

        en.clear() ; fr.clear() ; sameb.clear() ;

        cin >> n;

        cin >> s;
        all.insert(s) ;
        en.insert(s);
        while (scanf("%c", &c) != EOF) {
            if (c == '\n')
                break;
            cin >> s;
            all.insert(s) ;
            en.insert(s);
        }

        cin >> s;
        all.insert(s) ;
        fr.insert(s);
        while (scanf("%c", &c) != EOF) {
            if (c == '\n')
                break;
            cin >> s;
            all.insert(s) ;
            fr.insert(s);
        }

        n -= 2;

        FOR0(i, n)
            r[i].clear();
        FOR0(i, n) {
            cin >> s;
            all.insert(s) ;
            r[i].push_back(s) ;
            while (scanf("%c", &c) != EOF) {
                if (c == '\n')
                    break;
                cin >> s;
                all.insert(s) ;
                r[i].push_back(s) ;
            }
        }



        cnt = 0 ;
        for( it = all.begin() ; it != all.end() ; ++it ) {
            code[*it] = cnt ;
            ++cnt ;
        }

        ien.clear() ; ifr.clear() ; sameb.clear() ;
        for( it = en.begin() ; it != en.end() ; ++it )
            ien.insert( code[*it] ) ;
        for( it = fr.begin() ; it != fr.end() ; ++it ) {
            ifr.insert(code[*it]);
            if( ien.count(code[*it]) )
                sameb.insert(code[*it]) ;
        }

        FOR0(i,n) {
            ir[i].clear() ;
            FOR0(j, (int) r[i].size())
                ir[i].push_back( code[r[i][j]] ) ;
        }

        allr.clear() ;
        alln.clear() ;
        whon.clear() ;
        FOR0(i,n)
            FOR0(j, (int) r[i].size())
                allr.insert(ir[i][j]) ;
        for( set < int > :: iterator jt = allr.begin() ; jt != allr.end() ; ++jt ) {
            int val = *jt ;
            if( sameb.count(val) )
                continue ;
            vector < int > pos ;
            FOR0(i,n)
                FOR0(j, (int) r[i].size())
                    if( ir[i][j] == val ) {
                        pos.push_back(i);
                        break;
                    }
            alln.push_back(pos) ;

            if( ien.count(val) )
                whon.push_back(1) ;
            else if( ifr.count(val) )
                whon.push_back(2) ;
            else
                whon.push_back(0) ;
        }

        int w1, w2, cans, addans = (int)sameb.size() ;

        if( (int)alln.size() > 0 ) {
            ans = (int) 1e9;
            for (m = 0; m < (1 << n); ++m) {
                cans = 0;
                for (i = 0; i < (int) alln.size(); ++i) {
                    w1 = w2 = 0 ;
                    if( whon[i] ) {
                        if( whon[i] == 1 )
                            w1 = 1 ;
                        else
                            w2 = 1 ;
                    }

                    for (j = 0; j < (int) alln[i].size(); ++j) {
                        if ((1 << alln[i][j]) & m)
                            w1 = 1;
                        else
                            w2 = 1;
                        if (w1 && w2)
                            break;
                    }

                    if (w1 && w2)
                        ++cans;
                }
                ans = min(ans, cans);
            }
        }
        else
            ans = 0 ;

        cout << ans + addans << endl ;
    }
    return 0 ;
}