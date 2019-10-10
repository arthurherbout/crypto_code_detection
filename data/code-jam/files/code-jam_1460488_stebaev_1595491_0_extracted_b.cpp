// a.cpp : Defines the entry point for the console application.
//

#include <iostream>
using namespace std;
#include <string>
#include <set>
#include <string.h>
#include <vector>

void main()
{
	freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
	int T;
	cin>>T;
	for(int t=1; t<=T; t++)
	{
		int n, s, p;
		cin>>n>>s>>p;
		vector<int> mas(n);
		for(int i=0; i<n; i++)
			cin>>mas[i];
		int sup = 0, answer = 0;
		int toNormal = p*3-2, toSurprise = p*3-4;
		if(p==1) toSurprise = 1;
		for(int i=0; i<n; i++)
			if(mas[i]>=toNormal) answer++;
			else if(mas[i]>=toSurprise) sup++;
		answer+=min(s, sup);
		printf("Case #%d: %d\n", t, answer);
	}
}

