#include <stdio.h>
#include <vector>
#include <deque>
#include <algorithm>
#include <map>
#include <string>
//#include "bnum.h"
using namespace std;
#define MP make_pair
#define FS first
#define SD second
#define PI pair<int,int>
#define VI vector<int>
#define INF 1000000000
#define MOD %1000000007

typedef long long LL;

LL ile[2000];
int t,a,b,c;
int id;

LL A[2000];
LL ciag[2000];

int main() {
	scanf("%d",&t);
	id=1;
	while(t--) {
		LL n,m,X,Y,Z;
		scanf("%I64d %I64d%I64d%I64d%I64d",&n,&m,&X,&Y,&Z);
		for(int i=0;i<m;i++) scanf("%I64d",&A[i]);
		for (int i = 0 ; i<  n;i++) {
		  ciag[i]=A[i % m];
		  A[i % m] = (X * A[i % m] + Y * (i + 1)) % Z;
		}
		long long wynik=0;
		//for(int i=0;i<n;i++) printf("[%I64d]",ciag[i]);
		for(int i=0;i<2000;i++) ile[i]=1;
		for(int i=n-1;i>=0;i--) {
			for(int i2=i+1;i2<n;i2++) {
				if(ciag[i]<ciag[i2]) ile[i]=(ile[i]+ile[i2])MOD;
			}
			wynik=(wynik+ile[i])MOD;
		}
		printf("Case #%d: %I64d\n",id++,wynik);
	}


	return 0;
}