#include <bits/stdc++.h>
#define F first
#define S second
#define X real()
#define Y imag()
using namespace std;
typedef long long ll;
typedef long double ld;

queue<pair<vector<int>, int> > bfs;
set<vector<int> > used;

void put(vector<int> s, int d){
	if (used.count(s)) return;
	used.insert(s);
	bfs.push({s, d});
}

void solve(){
	int hd,ad,hk,ak,b,d;
	cin>>hd>>ad>>hk>>ak>>b>>d;
	int he=hd;
	while (!bfs.empty()) bfs.pop();
	used.clear();
	put({hd, ad, hk, ak}, 0);
	while (!bfs.empty()){
		auto t=bfs.front();
		bfs.pop();
		hd=t.F[0];
		ad=t.F[1];
		hk=t.F[2];
		ak=t.F[3];
		int dd=t.S;
		if (hd<=0) continue;
		if (hk<=0) {
			cout<<dd<<endl;
			return;
		}
		if (hk-ad<=0){
			put({hd, ad, hk-ad, ak}, dd+1);
		}
		put({hd-ak, ad, hk-ad, ak}, dd+1);
		put({hd-ak, ad+b, hk, ak}, dd+1);
		put({he-ak, ad, hk, ak}, dd+1);
		put({hd-max(0, ak-d), ad, hk, max(0, ak-d)}, dd+1);
	}
	
	cout<<"IMPOSSIBLE"<<endl;
}

int main(){
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	int tcs;
	cin>>tcs;
	for (int tc=1;tc<=tcs;tc++){
		cout<<"Case #"<<tc<<": ";
		solve();
	}
}