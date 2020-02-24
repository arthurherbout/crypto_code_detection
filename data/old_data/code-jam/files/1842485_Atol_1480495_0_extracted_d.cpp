/*{{{*/
/*includes e defines*/
#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <sstream>
using namespace std;
typedef long long LL;
typedef vector<int> VI;
typedef vector<string> VS;
typedef pair<int,int> pii;
#define sz(A) (int)(A).size()
#define FOR(A,B) for(int A=0; A < (int) (B);A++)
#define FOREACH(A,B) for(__typeof((B).begin()) A = (B).begin(); A != (B).end(); A++)
#define pb push_back
#define all(x) x.begin() , x.end()
#define mp make_pair
/*}}}*/
/*{{{*/
/*main*/
void solveCase();
int main() {
	int TESTES; scanf("%d", &TESTES);
	for(int TESTE = 1; TESTE <= TESTES; TESTE++) {
		printf("Case #%d: ", TESTE);
		solveCase();
	}
    return 0;
}
/*}}}*/

int R,C;
string tab[16];
int vai[16][16];

int da[] = {0,0,-1};
int db[] = {-1,1,0};

void solve(int r, int c) {
	printf("%c: ", tab[r][c]);
	memset(vai, 0, sizeof(vai));

	queue< pii > fila;
	set< pii > ja;

	ja.insert( mp(r,c) );
	fila.push( mp(r,c) );
	while(!fila.empty()) {
		pii at = fila.front(); fila.pop();
		int a = at.first, b = at.second;
		FOR(i, 3) {
			int aa = a + da[i], bb = b + db[i];
			if(tab[aa][bb] != '#' && !ja.count( mp(aa,bb) )) {
				ja.insert( mp(aa,bb) ) ;
				fila.push( mp(aa,bb) );
			}
		}
	}

	set< set< pii > > conj;
	set< pii > v;
	for(set<pii>::iterator it = ja.begin(); it != ja.end(); it++) v.insert( *it );
	conj.insert( v );

	vector< set<pii> > posso;
	posso.pb( v );


	set< pii > fim;
	fim.insert( mp(r,c) );

	while(sz(posso) && !conj.count(fim)) {
		v = posso.back(); posso.pop_back();
		
		FOR(i, 3) {
			set< pii > novo;
			int ferrou = 0;
			FOREACH(k, v) {
				int a = k->first - da[i];
				int b = k->second - db[i];
				if(tab[a][b] == '#') {
					a = k->first;
					b = k->second;
				}
				if(!ja.count(mp(a,b))) ferrou = 1;
				novo.insert( mp(a,b));
			}
			if(!ferrou && !conj.count(novo)) {
				conj.insert(novo);
				posso.pb(novo);
			}
		}

	}

	printf("%d ", sz(ja));
	if(conj.count(fim)) printf("Lucky\n");
	else printf("Unlucky\n");

}

void solveCase() {
	cin >> R >> C;
	FOR(i, R) cin >> tab[i];

	printf("\n");

	FOR(c, 10) {

		FOR(i, R) {
			FOR(j, C) {
				if(tab[i][j] == c + '0') {
					solve(i,j);
				}
			}
		}

	}

}


