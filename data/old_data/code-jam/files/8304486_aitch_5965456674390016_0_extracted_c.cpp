#include <bits/stdc++.h>
using namespace std;
#define rep(i,a,b) for(int i = a; i < (b); ++i)
#define rrep(i,a,b) for(int i = b; i --> (a);)
#define all(v) v.begin(),v.end()
#define trav(x,v) for(auto &x : v)
#define sz(v) (int)(v).size()
typedef long long ll;
typedef vector<int> vi;
typedef pair<int,int> pii;

struct UF {
	vi t;
	UF(int n){
		t.resize(n,-1);
	};
	int find(int a){
		return t[a]<0 ? a : t[a] = find(t[a]);
	}
	bool uni(int a, int b){
		a = find(a), b = find(b);
		if(a==b) return 0;
		t[a] = b;
		return 1;
	}
};

int dist(int a, int b){
	int ans = b-a;
	if(ans < 0) ans += 24;
	return ans;
}

void solve(){
	int c;
	cin >> c;
	int m = 2*c;
	vi as(m), bs(m), ls(m), ds(m);
	rep(i,0,m){
		as[i] = i/2;
		cin >> bs[i] >> ls[i] >> ds[i];
		--bs[i];
	}
	ll ans = 0;
	trav(x, ds) ans += x;
	vector<vi> in(c), ut(c);
	rep(i,0,m) in[bs[i]].push_back(i);
	rep(i,0,m) ut[as[i]].push_back(i);

	UF uf(m);

	priority_queue<pair<int,pii>> ko;

	rep(i,0,c){
		vi t1, t2;
		trav(e, in[i]) t1.push_back((ls[e] + ds[e])%24);
		trav(e, ut[i]) t2.push_back(ls[e]);
		int c1,c2;
		if(i != 0){
			c1 = dist(t1[0], t2[0]) + dist(t1[1], t2[1]);
			c2 = dist(t1[0], t2[1]) + dist(t1[1], t2[0]);
		} else {
			c1 = min(dist(0, t2[0]) + dist(t1[1], t2[1]),
					 dist(0, t2[1]) + dist(t1[0], t2[0]));
			c2 = min(dist(0, t2[0]) + dist(t1[0], t2[1]),
					 dist(0, t2[1]) + dist(t1[1], t2[0]));
		}
		ans += min(c1, c2);
		ko.push({-abs(c2-c1), {in[i][0], in[i][1]}});
		if(c1 < c2){
			uf.uni(in[i][0], ut[i][0]);
			uf.uni(in[i][1], ut[i][1]);
		} else {
			uf.uni(in[i][1], ut[i][0]);
			uf.uni(in[i][0], ut[i][1]);
		}
	}
	while(!ko.empty()){
		auto pa = ko.top();
		ko.pop();
		pii ab = pa.second;
		int cost = -pa.first;
		if(uf.uni(ab.first, ab.second)){
			ans += cost;
		}
	}
	cout << ans << endl;
}

int main(){
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	
	int n;
	cin >> n;
	rep(t,1,n+1){
		cout << "Case #" << t << ": ";
		solve();
	}
}