#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

int n,m;
string mix;
map<string, int> d;
map<string, vector<string> > a;

int solve(string node) {
	if (a[node].size()==0) {
		if (islower(node[0]))
			return (d[node]=(int)-1e9);
		if (isupper(node[0]))
			return (d[node]=1);
	}
	vector<int> b;
	int i,s;
	s=a[node].size();
	for (i=0;i<s;i++)
		b.push_back(solve(a[node][i]));
	sort(b.begin(),b.end());
	reverse(b.begin(),b.end());
	d[node]=1;
	for (i=0;i<s;i++)
		if (b[i]!=(int)-1e9)
			d[node]++;
	s=b.size();
	for (i=0;i<s;i++)
		d[node]=max(d[node],i+b[i]);		
	return d[node];
}

int main () {
	int i,w,tests;
	string root,tmp;
	freopen("in.txt","r",stdin);
	freopen("out.txt","w",stdout);
	for (cin >> tests,w=1;w<=tests;w++) {
		cout << "Case #" << w << ": ";
		cin >> n;
		for (i=1;i<=n;i++) {
			cin >> mix;
			if (i==1) 
				root=mix;
			for (cin >> m;m>0;m--) {
				cin >> tmp;
				a[mix].push_back(tmp);			
			}
		}
		cout << max(solve(root),0) << endl;
		d.clear();
		a.clear();
	}
	return 0;
}