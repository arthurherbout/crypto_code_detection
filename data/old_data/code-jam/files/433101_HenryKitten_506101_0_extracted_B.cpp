#include <stdio.h>
#include <iostream>
#include <algorithm>
using namespace std;

int main(){
	int ncas;
	cin >> ncas;
	for(int c=1;c<=ncas;c++){
		int n;
		int t[10];
		cin >> n;
		for(int i=0;i<n;i++) cin >> t[i];
		sort(t,t+n);
		int g = t[1] - t[0];
		for(int i=1;i<n-1;i++) g = __gcd(g,t[i+1]-t[i]);
		int p = - (t[0]%g); 
		if(p<0) p += g;
		printf("Case #%d: %d\n",c,p);
	}
	return 0;
}

