#include <iostream>
#include <string>
#include <cstring>
#include <string.h>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <bitset>
#include <math.h>

using namespace std;

char s[200];
long long pw[200];

void solve(int tst){
	printf("Case #%d: ",tst);
	gets(s);

	int cur=0;
	int n=strlen(s);
	long long tek=0;
	vector <long long> add;
	for (int i=0; i<n; i++){
		if (s[i]=='?') cur++, add.push_back(pw[n-i-1]); else
			if (s[i]=='1') tek+=pw[n-i-1];
	}

	for (int i=0; i<(1<<cur); i++){
		bitset<25> b=i;
		long long nwcur=tek;
		for (int j=0; j<cur; j++)
			if (b[j]) nwcur+=add[j];

		long long sqr=sqrt((double)(nwcur));
		int ok=0;
		for (long long k=sqr-10LL; k<=sqr+10LL; k++)
			if (k*k==nwcur){
				ok=1;
				break;
			}
		if (ok){
			int curv=0;
			for (int j=0; j<n; j++)
				if (s[j]=='?'){
					printf("%d",(int)b[curv]);
					curv++;
				} else
					printf("%c",s[j]);
				printf("\n");
				break;
		}
	}
}

int main(){
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	int tests;
	scanf("%d\n",&tests);
	pw[0]=1LL;
	for (int i=1; i<200; i++)
		pw[i]=2LL*pw[i-1];

	for (int tt=1; tt<=tests; tt++){
		solve(tt);
		cerr<<tt<<endl;
	}

	return 0;
}
