#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std ;

int N , M , A[110][110] ;
char B[110][110] ;
int P[440] , Q[440] ;

void Init() {
	cin >> N >> M ; memset(A , 0 , sizeof(A)) ;
	int cnt = 0 ;
	for ( int i = 1 ; i <= M ; i ++ ) { P[++cnt] = 0 ; Q[cnt] = i ; }
	for ( int i = 1 ; i <= N ; i ++ ) { P[++cnt] = i ; Q[cnt] = M+1 ; }
	for ( int i = M ; i >= 1 ; i -- ) { P[++cnt] = N+1 ; Q[cnt] = i ; }
	for ( int i = N ; i >= 1 ; i -- ) { P[++cnt] = i ; Q[cnt] = 0 ; }
	
	int cnt2 = 0 ;
	for ( int i = 1 ; i <= N+M ; i ++ ) {
		int u , v ; cin >> u >> v ;
		A[P[u]][Q[u]] = ++cnt2 ;
		A[P[v]][Q[v]] =   cnt2 ;
	}
}

int GAO( int x , int y , int u , int v ) {
	for ( ; A[u][v] == 0 ; ) {
		int i , j ; //int tx, ty;
		if ( B[u][v] == '/' ) {
			if      ( u == x + 1 ) { i = u ; j = v - 1 ; }
			else if ( u == x - 1 ) { i = u ; j = v + 1 ; }
			else if ( v == y + 1 ) { i = u - 1 ; j = v ; }
			else if ( v == y - 1 ) { i = u + 1 ; j = v ; }
		} else {
			if      ( u == x + 1 ) { i = u ; j = v + 1 ; }
			else if ( u == x - 1 ) { i = u ; j = v - 1 ; }
			else if ( v == y + 1 ) { i = u + 1 ; j = v ; }
			else if ( v == y - 1 ) { i = u - 1 ; j = v ; }
		}
		x = u ; y = v ; u = i ; v = j ;
	}
	return A[u][v] ;
}

#define backornot if ( st != ed ) return false

bool Check() {
	for ( int i = 1 ; i <= M ; i ++ ) { int st = A[0][i] , ed = GAO(0 , i , 1 , i) ; backornot ; }
	for ( int i = 1 ; i <= M ; i ++ ) { int st = A[N + 1][i] , ed = GAO(N + 1 , i , N , i) ; backornot ; }
	for ( int i = 1 ; i <= N ; i ++ ) { int st = A[i][0] , ed = GAO(i , 0 , i , 1) ; backornot ; }
	for ( int i = 1 ; i <= N ; i ++ ) { int st = A[i][M + 1] , ed = GAO(i , M + 1 , i , M) ; backornot ; }
	return true;
}


void Output() {
	for ( int i = 1 ; i <= N ; i ++ ) {
		for ( int j = 1 ; j <= M ; j ++ )
			cout << B[i][j] ;
		cout << "\n" ;
	}
}

bool Search( int i , int j ) { // row i, column j
	if ( i > N ) {
		if ( Check() ) { Output() ; return true ; }
		else           return false ;
	}
	
	int Nexti = i , Nextj = j + 1 ;
	if ( Nextj == M + 1 ) {
		Nexti ++ ;
		Nextj = 1 ;
	}
	
	B[i][j] = '\\' ;
	if ( Search(Nexti , Nextj) ) return true ;
	
	B[i][j] = '/';
	if ( Search(Nexti , Nextj) ) return true ;
	
	return false ;
}

int main() {
	freopen("C-small-attempt0.in" , "r" , stdin) ;
	freopen("C-small-attempt0.out", "w" ,stdout) ;
	
	int Test ; cin >> Test ;
	for ( int i = 1 ; i <= Test ; i ++ ) {
		Init() ;
		cout << "Case #" << i << ":\n" ;
		if ( !Search(1 , 1) ) puts("IMPOSSIBLE") ;
	}
}