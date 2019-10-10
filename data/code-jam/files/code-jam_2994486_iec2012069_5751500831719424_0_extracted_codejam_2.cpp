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
	int k;
	freopen("A-small-attempt0.in", "r", stdin);
	freopen("output.txt", "w", stdout);
	cin  >> t;
	for(k = 1; k <= t; k++) {
		string s1;
		int n;
		cin >> n;
		string s2;
		string s3;
		string s4;
		cin >> s1 >> s2;
		s3 = s4 = "";
		int a[1000] = {0};
		int b[1000] = {0};
		int c[1000] = {0};
		int l1;
		int l2;
		int j = 0;
		int i;
		a[0] = 1;
		s3 = s3 + s1[0];
		for(i = 1; i < s1.length(); i++) {
			if(s1[i] == s1[i-1]) {
				a[j]++;
			} else {
				j++;
				a[j] = 1;
				s3 = s3 + s1[i];
			}	
		}
		j = 0;
		b[0] = 1;
		s4 = s4 + s2[0];
		for(i = 1; i < s2.length(); i++) {
			if(s2[i] == s2[i-1]) {
				b[j]++;
			} else {
				j++;
				b[j] = 1;
				s4 = s4 + s2[i];
			}	
		}
		int m;
		int max = 0;
		if(s3.compare(s4) == 0) {
			for(i = 0; i <= j; i++) {
				m = a[i] - b[i];
				if(m < 0) {
					m = m * -1;
				}
				
					max =max + m;
				
			}
		} else {
			cout << "Case #" << k <<": ";
			cout << "Fegla Won\n";
			continue;  
		}
		cout << "Case #" << k <<": ";  
		cout << max << endl;
	}

	return 0;
}

