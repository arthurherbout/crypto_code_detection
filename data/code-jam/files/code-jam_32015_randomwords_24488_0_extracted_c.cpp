#include <algorithm>
#include <numeric>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cassert>

#include <cmath>
#include <complex>

using namespace std;

long long dp[5000];
long long B[5000];
long long A[5000];

long long num(long long n, long long lim){
	long long res = 1;
	if(dp[n] != -1)
		return dp[n];
	long long a = B[n];
	for(long long i=n+1; i<lim; i++){
		if (a < B[i])
			res = (res + num(i, lim)) % 1000000007;
	}
	dp[n] = res % 1000000007;
	return dp[n];
}
			
int main(void){
	long long T;
	cin >> T;
	for(long long q = 0; q < T; q++){
		memset(&dp, -1, sizeof(dp));
		long long res = 0;
		long long n,m,X,Y,Z;
		cin >> n >> m >> X >> Y >> Z;
		//cout << n << " " << m << " " << X << " " << Y << " " << Z << endl;
		for(long long i=0; i<m; i++){
			scanf("%d", &A[i]);
			//cout << A[i] << " ";
		}
		//cout << endl;
		for(long long i=0; i<n; i++){
			B[i] = A[i % m];
			//cout << B[i] << " ";
			A[i % m] = (X * A[i % m] + Y * (i + 1)) % Z;
		}
		////cout << endl;
		for(long long i=0; i<n; i++){
			res = (res + num(i, n)) % 1000000007;
		}
		for(long long i=0; i<n; i++){
			//cout << dp[i] << " ";
		}
		//cout << endl;
		cout << "Case #" << q+1 << ": " << res % 1000000007 << endl;
	}
}
