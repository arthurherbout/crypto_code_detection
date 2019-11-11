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
typedef pair<int,int> PII;
#define sz(A) (int)(A).size()
#define FOR(A,B) for(int A=0; A < (int) (B);A++)
#define FOREACH(A,B) for((__typeof (B).begin) A = (B).begin(); A != (B).end(); A++)
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

typedef pair<double, double> pdd;

void solveCase() {
	vector< pdd > p[2];
	int n;
	cin >> n;
	FOR(i, n) {
		char c;
		int s, at;
		cin >> c >> s >> at;
		if(c == 'L') p[0].pb( mp(at, s) );
		else p[1].pb( mp(at, s) );
	}

	double soma = 0;
	int contador = 0;
	double eps = 1e-12;
	eps = 1e-9;

	while(1) {

		if(contador++ >= 1000000) {
			printf("%.10f\n", soma);
			break;
		}

		double tcol = 1e18;
		int ii = -1, jj = -1;
		FOR(j, 2) {
			sort( all(p[j]) );
			FOR(i, sz(p[j])-1) {
				double at1 = p[j][i].first, at2 = p[j][i+1].first;
				double v1 = p[j][i].second, v2 = p[j][i+1].second;

				if(v1 <= v2) continue;
				double tempo = (at2 - at1 - 5) / (v1 - v2);
				if(tempo <= tcol) {
					tcol = tempo;
					jj = j;
					ii = i;
				}
			}
		}


		if(tcol >= 1e18) {
			cout << "Possible" << endl;
			break;
		}

		soma += tcol;
		FOR(j, 2) {
			FOR(i, sz(p[j])) {
				p[j][i].first += p[j][i].second * tcol; 
			}
		}


		pdd sai = p[jj][ii];

		double ok = true;
		double gambi = false;
		int j = (jj^1);

		FOR(i, sz(p[j])) {
			double at1 = p[j][i].first, at2 = sai.first;
			double v1 = p[j][i].second, v2 = sai.second;

			if( fabs(at2 - at1) < 5 - eps) {
				ok = false;
			}
		}


		if(ok) {
			p[jj].erase( p[jj].begin() + ii );
			p[j].pb( sai );
			continue;
		}


		sai = p[jj][ii+1];

		FOR(i, sz(p[j])) {
			double at1 = p[j][i].first, at2 = sai.first;
			if( fabs(at2 - at1) < 5 - eps) {
				ok = false;
			}
		}

		if(ok) {
			p[jj].erase( p[jj].begin() + ii + 1);
			p[j].pb( sai );
			continue;
		}

		ok = true;
		sai = p[jj][ii];


		FOR(i, sz(p[j])) {
			double at1 = p[j][i].first, at2 = sai.first;
			double v1 = p[j][i].second, v2 = sai.second;

			if( fabs(at2 - at1) < 5 - eps) {

				if(at1 < at2 && v1 > v2) {
					at1 -= tcol * p[j][i].second;
					at2 -= tcol * sai.second;
					if(fabs(at1 - at2) >= 5) {
						double tempo = (fabs(at2 - at1) - 5) / fabs(v1 - v2);

						if(tempo > eps) {

							at1 += tempo * p[j][i].second;
							at2 += tempo * sai.second;
							bool posso = true;
							FOR(x, sz(p[j])) {
								double aaa = p[j][x].first - (tcol+tempo) * p[j][x].second;
								if( fabs(aaa - at2) < 5 -eps) posso = false;
							}
							FOR(x, sz(p[jj])) {
								double aaa = p[jj][x].first - (tcol+tempo) * p[jj][x].second;
								if( fabs(aaa - at1) < 5 - eps) posso = false;
							}
							if(posso) {
//								cerr << "NOVO era " << tcol << " agora eh " << tempo << endl;

								soma -= tcol;

								FOR(jjj, 2) {
									FOR(iii, sz(p[jjj])) {
										p[jjj][iii].first -= p[jjj][iii].second * tcol; 
									}
								}

								tcol = tempo;
								soma += tcol;

								FOR(jjj, 2) {
									FOR(iii, sz(p[jjj])) {
										p[jjj][iii].first -= p[jjj][iii].second * tcol; 
									}
								}
								pdd novo = p[j][i];
								p[j].erase( p[j].begin() + i);
								p[jj].erase( p[jj].begin() + ii );
								p[j].pb( sai );
								p[jj].pb( novo );
								gambi = true;
								break;
							}
						}
					}

				}
				ok = false;
				break;
			}
		}

		if(ok && !gambi) {
			p[jj].erase( p[jj].begin() + ii );
			p[j].pb( sai );
			continue;
		}
		if(ok) continue;

		ok = true;
		sai = p[jj][ii+1];

		FOR(i, sz(p[j])) {
			double at1 = p[j][i].first, at2 = sai.first;
			double v1 = p[j][i].second, v2 = sai.second;

			if( fabs(at2 - at1) < 5 - eps ) {

				if(at1 > at2 && v2 > v1) {
					at1 -= tcol * p[j][i].second;
					at2 -= tcol * sai.second;
					if(fabs(at1 - at2) >= 5) {
						double tempo = (fabs(at2 - at1) - 5) / fabs(v1 - v2);

						if(tempo > eps) {

							at1 += tempo * p[j][i].second;
							at2 += tempo * sai.second;
							bool posso = true;
							FOR(x, sz(p[j])) {
								double aaa = p[j][x].first - (tcol+tempo) * p[j][x].second;
								if( fabs(aaa - at2) < 5 - eps) posso = false;
							}
							FOR(x, sz(p[jj])) {
								double aaa = p[jj][x].first - (tcol+tempo) * p[jj][x].second;
								if( fabs(aaa - at1) < 5 -eps) posso = false;
							}
							if(posso) {
//								cerr << "NOVO era " << tcol << " agora eh " << tempo << endl;

								soma -= tcol;

								FOR(jjj, 2) {
									FOR(iii, sz(p[jjj])) {
										p[jjj][iii].first -= p[jjj][iii].second * tcol; 
									}
								}

								tcol = tempo;
								soma += tcol;

								FOR(jjj, 2) {
									FOR(iii, sz(p[jjj])) {
										p[jjj][iii].first -= p[jjj][iii].second * tcol; 
									}
								}
								pdd novo = p[j][i];
								p[j].erase( p[j].begin() + i);
								p[jj].erase( p[jj].begin() + ii + 1 );
								p[j].pb( sai );
								p[jj].pb( novo );
								gambi = true;
								break;
							}
						}
					}

				}
				ok = false;
				break;
			}
		}



		if(ok && !gambi) {
			p[jj].erase( p[jj].begin() + ii + 1);
			p[j].pb( sai );
			continue;
		}
		if(ok) continue;



		if(!ok) {
			printf("%.10f\n", soma);
			break;
		}



	}
	return;


	cerr << n << endl;
	FOR(j, 2) {
		FOR(i, sz(p[j])) {
			cerr << "("<< p[j][i].first << "," << p[j][i].second << ");;";
		}
		cerr << endl;
	}

}

