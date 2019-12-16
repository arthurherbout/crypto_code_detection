#include <iostream>
#include <algorithm>

const int MAX = 20;
const int MOD = 1000000009;

int N, K;
int A[MAX], B[MAX];
bool C[MAX][MAX];
long long E[1<<MAX];

void ReadData()
{
	memset( C, 0, sizeof(C) );
	std::cin >> N >> K;
	for( int i = 0; i<N-1; ++i )
	{
		std::cin >> A[i] >> B[i];
		--A[i];
		--B[i];
	}
}

long long Calc( int m, int k )
{
	int r = K;
	for( int i = 0; i<N-1; ++i )
		if( C[k][i] && (m&(1<<i)) )
			--r;
	if( r<=0 )
		return 0;
	return E[m]*r%MOD;
}

int Work()
{
	bool D[MAX][MAX];
	memset( D, 0, sizeof(D) );
	memset( E, 0, sizeof(E) );
	for( int i = 0; i<N-1; ++i )
		for( int j = 0; j<N-1; ++j )
		{
			if( A[i]==A[j] || A[i]==B[j] || B[i]==A[j] || B[i]==B[j] )
				C[i][j] = C[j][i] = true;
		}
	for( int i = 0; i<N-1; ++i )
		for( int j = 0; j<N-1; ++j )
			for( int k = 0; k<N-1; ++k )
				if( C[i][j] && C[j][k] )
					D[i][k] = true;
	for( int i = 0; i<N-1; ++i )
		for( int j = 0; j<N-1; ++j )
			if( D[i][j] )
				C[i][j] = true;
	E[0] = 1;
	for( int i = 0; i<(1<<(N-1)); ++i )
	{
		for( int j = 0; j<N-1; ++j )
			if( (i&(1<<j))==0 )
				E[i|(1<<j)] = Calc( i, j );
	}
	return E[(1<<(N-1))-1];
}

void Output( int t, int res )
{
	std::cout << "Case #" << t << ": ";
	if( res >=0 )
		std::cout << res << std::endl;
	else
		std::cout << "IMPOSSIBLE" << std::endl;
}

int main()
{
	int n;
	std::cin >> n;
	for( int i = 1; i<=n; ++i )
	{
		ReadData();
		Output( i, Work() );
	}
	return 0;
}
