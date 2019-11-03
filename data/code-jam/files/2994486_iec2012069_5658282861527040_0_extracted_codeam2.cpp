#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <set>
#include <sstream>
#include <cmath>
#define ull unsigned long long int
#define ll long long
#define Max(a,b) a >b ? a :b
#define S(n) scanf("%d",&n)
#define Sl(n) scanf("%ld",&n)
#define Sll(n) scanf("%lld",&n)
#define li long int
using namespace std;
int main()
{
	int t;
	freopen("B-small-attempt0.in", "r", stdin);
	freopen("output.txt", "w", stdout);
	cin >> t;
	int l;
	for(l = 1; l <= t; l++) {
		
	

	int a;
	int b;
	int k;
	int i;
	int j;
	cin >> a >> b >> k;
	int c = 0;
	for(i = 0; i < a; i++) {
		for(j =0; j < b; j++) {
			if((i& j) < k) {
				c++;
			} 
		}
	}
	cout << "Case #" << l <<": ";  
	cout << c << endl;
	}
	return 0;
}

