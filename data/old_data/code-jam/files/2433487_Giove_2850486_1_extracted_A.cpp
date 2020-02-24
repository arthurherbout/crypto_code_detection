#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <string>
#include <set>
#include <map>


using namespace std;

long long int B;
int N;
long long int puntate[37];
long long int somme[37];

map<long long int, int> quanti;

long long int da_usare ( int q, int r, long long int h ) {
	return ( (q+r)*h + r - somme[q+r-1] );
}

bool possibile ( int q, int r, long long int h ) {
	return ( da_usare(q,r,h) <= B );
}

double miglior_profitto;

double profitto ( int q, int r, long long int h, int altri ) {
	double res = ( 36 / ((double)(q+altri))) * (q*h-somme[q-1]) - ( (q+r)*h + r - somme[q+r-1] );
	
	if ( res > miglior_profitto ) {
		//printf("q=%d, r=%d, h=%lld, altri=%d. Nuovo profitto: %lf\n", q,r,h,altri,res);
		miglior_profitto = max( miglior_profitto, res );
	}
}

long long int ricerca ( long long int x, long long int y, int q, int r ) {
	if ( x == y ) return x;
	
	long long int m = (x+y+1)/2;
	if ( possibile(q,r,m) ) {
		return ricerca ( m, y, q, r );
	}
	else return ricerca ( x, m-1, q, r );
}



void solve() {
	scanf("%lld %d", &B, &N);
	
	for (int i=0; i<37; ++i) {
		puntate[i] = 0;
		somme[i] = 0;
	}
	
	for (int i=0; i<N; ++i) {
		long long int y;
		scanf("%lld",&y);
		puntate[i] = y;
	}
	
	sort( puntate, puntate+37 );
	
	somme[0] = puntate[0];
	for (int i=1; i<37; ++i) {
		somme[i] = somme[i-1] + puntate[i];
	}
	
	miglior_profitto = 0;
	
	for (int q=1; q<37; ++q) {
		for (int r=0; q+r<=36; ++r) {
			
			long long int a,b;
			a = puntate[q-1];
			b = puntate[q+r-1];
			
			long long int hmin = max( a, b-1 );
			long long int hmax = puntate[ q+r ]; // provare sia lui che lui-1, se possibile
			
			if ( !possibile(q,r,hmin) ) continue;
			
			if ( possibile(q,r,hmax-1) ) {
				profitto(q,r,hmax-1,0);
				
				// Provo hmax
				
				if ( possibile(q,r,hmax) ) {
					int altri = 0;
					for ( int i=q+r; i<37; ++i) {
						if ( puntate[i] == hmax ) altri++;
					}
				
					profitto(q,r,hmax,altri);
				}
			}
			else {
				
				// Trovo l'h più alto possibile
				
				long long int h = ricerca( hmin, hmax, q, r );
				assert ( possibile(q,r,h) );
				
				profitto(q,r,h,0);
			}
			
		}
	}
	
	printf("%.10lf\n", miglior_profitto);
}


void solve2() {
	
	scanf("%lld %d", &B, &N);
	
	for (int i=0; i<37; ++i) puntate[i] = 0;
	
	for (int i=0; i<N; ++i) {
		long long int y;
		scanf("%lld",&y);
		puntate[i] += y;
	}
	
	quanti.clear();
	
	int minimo = puntate[0]+1;
	
	for (int i=0; i<37; ++i) {
		//if ( quanti.count( puntate[i] ) == 0 ) quanti[ puntate[i] ] = 0;
		//quanti[puntate[i]]++;
		
		if ( puntate[i] < minimo ) {
			minimo = puntate[i];
		}
	}
	
	if ( minimo > 0 ) {
		for (int i=0; i<37; ++i) {
			puntate[i] -= minimo;
		}
	}
	
	for (int i=0; i<37; ++i) {
		if ( quanti.count( puntate[i] ) == 0 ) quanti[ puntate[i] ] = 0;
		quanti[puntate[i]]++;
	}
	
	double profitto = 0;
	long long int messi = 0;
	
	assert ( quanti.count(0) > 0 );
	
	int livello = quanti[0];
	
	
	for ( map<long long int,int>::iterator i = quanti.begin(); i != quanti.end(); i++ ) {
		// printf("%lld\n",(*i).first);
		map<long long int,int>::iterator j = i;
		j++;
		
		j++;
		if ( j == quanti.end() ) {
			break;
		}
		j--;
		
		//printf("%lld, %lld, %d.\n", (*j).first, (*i).first, (*i).second);
		
		long long int diff = (*j).first - (*i).first;
		long long int q = B / (*i).second;
		long long int veroq = min( q, diff );
		
		//printf("Diff: %lld. q=%lld. veroq=%lld\n", diff, q, veroq);
		
		bool raggiunto = ( q >= diff ); // Ho raggiunto il livello dopo
		
		if ( veroq > 1 ) {
			veroq -= 1;
			raggiunto = false;
		}
		
		// Metto veroq su ciascuno
		B -= veroq * (*i).second;
		messi += veroq * (*i).second;
		
		//printf("Messi: %lld\n", messi);
		
		if (raggiunto) {
			// Ho raggiunto il livello dopo
			livello += (*j).second;
			//printf("DELTA: %d\n", (*j).second);
		}
		
	
		//printf("Livello: %d\n", livello);
		
		double p = 1 / ( (double)(livello) );
		double prova = messi*(36*p-1);
		profitto = max( profitto, prova );
		//printf("NUOVO: %lf\n", prova);
	}
	/*
	printf("Livello: %d\n", livello);
	
	double p = 1 / ( (double)(livello) );
	profitto = messi*(36*p-1);
	*/
	printf("%.10lf\n",profitto);
}

int main() {
	
	int t;
	scanf("%d",&t);
	
	for (int i=0; i<t; ++i) {
		printf("Case #%d: ",i+1);
		solve();
	}
	
	return 0;
}
