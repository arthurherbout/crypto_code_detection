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

void solve(){
	int n,m;
	cin >> n >> m;
	vi as(m), bs(m), ws(m);
	rep(i,0,m) cin >> as[i] >> bs[i];
	trav(x, as) --x;
	trav(x, bs) --x;

	vector<vector<pair<int,bool>>> gr(n);
	rep(i,0,m){
		gr[as[i]].emplace_back(i, 0);
		gr[bs[i]].emplace_back(i, 1);
	}

	rep(e,0,m) if(ws[e] == 0){
		int a = as[e], b = bs[e];
		vector<bool> vis(n);
		vector<pair<int,bool>> path(1, {e,0});
		bool success = 0;
		function<void(int)> dfs = [&](int v){
			vis[v] = 1;
			if(v == a){
				set<int> s;
				s.insert(0);
				trav(pa, path){
					s.insert(ws[pa.first]*(pa.second ? -1 : 1));
				}
				int w = 1e9;
				rep(i,-1010, 1010) if(abs(w) > abs(i) && !s.count(i)){
					w = i;
				}
				assert(w != 1e9);
				trav(pa, path){
					ws[pa.first] -= w*(pa.second ? -1 : 1);
				}
				success = 1;
				return;
			} else {
				trav(pa, gr[v]) if(pa.first != e){
					int u = (pa.second ? as : bs)[pa.first];
					if(vis[u]) continue;
					path.push_back(pa);
					dfs(u);
					if(success) return;
					path.pop_back();
				}
			}
		};
		dfs(b);
		if(!success){
			cout << "IMPOSSIBLE" << endl;
			return;
		}
	}

	vi deg(n);
	rep(i,0,m){
		deg[as[i]] += ws[i];
		deg[bs[i]] -= ws[i];
	}
	trav(x, deg) assert(x == 0);

	trav(x, ws){
		assert(abs(x) <= n*n);
		assert(x != 0);
		cout << x << ' ';
	}
	cout << endl;
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