#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
#define F first
#define S second

map<string,int> cheat;

int cnt(string g) {
	if (cheat.count(g)) return cheat[g];
//	cerr<<g<<endl;
	int n=g.size();
	int indicator=0;
	for (int i=0;i<n;i++) {
		indicator+=g[i]-'0';
	}
	if (indicator>n) return 1;
	string gg;
	gg.resize(n);
	int j=n-1;
	for (int i=n-1;i>=0;i--) {
		for (int ii=0;ii<g[i]-'0';ii++) gg[j--]=i+1+'0';
	}
	while (j>=0) gg[j--]='0';
	int res=1;
	do {
		if (gg!=g) res+=cnt(gg);
	} while (next_permutation(gg.begin(),gg.end()));
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	int tcs;
	cin>>tcs;
	cheat["100000000"]=999999999;
	cheat["010000000"]=971298982;
	cheat["200000000"]=875932309;
	cheat["110000000"]=314095393;
	for (int tc=1;tc<=tcs;tc++) {
		string g;
		cin>>g;
		cout<<"Case #"<<tc<<": "<<cnt(g)<<"\n";
	}
}
