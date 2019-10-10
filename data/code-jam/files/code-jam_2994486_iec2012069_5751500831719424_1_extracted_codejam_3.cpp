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
	freopen("A-large.in", "r", stdin);
	freopen("output.txt", "w", stdout);
	int t;
	int k;
	cin >> t;
	for(k = 1; k <= t; k++) {
		int n;
		cin >> n;
		
		string s1[110];
		string s2[110];
		int i;
		int j;
		int a[110][110];
		int b[110][110];
		for(i = 0; i <= 100; i++) {
			for(j = 0; j<= 100; j++) {
				a[i][j] = b[i][j] = 0;
			}
		}
	
		int l;
		for(i = 0; i < n; i++) {
			//cout << "asi\n";
			cin >> s1[i];
			//cout << "asdl\n";
			s2[i][0] = s1[i][0];
			a[i][0] = 1;
		}
		l = 0;
		for(i = 0; i < n; i++) {
			l = 0;
			for(j = 1; j < s1[i].length(); j++) {
				if(s1[i][j] == s1[i][j-1]) {
					a[i][l]++;
				} else {
					l++;
					s2[i] = s2[i] + s1[i][j];
					a[i][l] = 1;	
				}
			} 
		}
		int c = 0;
		for(i = 1; i < n; i++) {
			if(s2[i].compare(s2[i-1]) != 0) {
				c = 1;
				break;
			}
		}
	
		int ans = 0;
		cout << "Case #" << k << ": " ;
		if(c == 1) {
			cout << "Fegla Won\n";
		} else {
			int temp[1000];
			int p = 0;
			for(i = 0; i <= l; i++) {
				p = 0;
				for(j = 0; j < n; j++) {
					temp[p++]  = a[j][i];
				}
				sort(temp,temp+n);
				int z  = temp[n/2];
				for(j = 0; j < n; j++) {
					int g = z - temp[j];
					if(g < 0) {
						g = g * -1;
					} 
					ans = ans + g;
				}
			}
			cout << ans << endl;
		}
		
		
	}
	return 0;
}

