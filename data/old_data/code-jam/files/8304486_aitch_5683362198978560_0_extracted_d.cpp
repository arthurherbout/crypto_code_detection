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

int dx[] = {0,0,1,-1}, dy[] = {1,-1,0,0};

ll mod = 1e9+7;

void solve(){
	int r,c,n,d;
	cin >> r >> c >> n >> d;

	vector<vector<ll>> col(r, vector<ll>(c,-1));
	priority_queue<pair<ll,pii>> ko;

	rep(_,0,n){
		int x,y,b;
		cin >> x >> y >> b;
		col[x-1][y-1] = b;
		ko.push({-b, {x-1,y-1}});
	}

	while(!ko.empty()){
		auto pa = ko.top(); ko.pop();
		int x = pa.second.first, y = pa.second.second;
		ll b = -pa.first;
		rep(t,0,4){
			int x1 = x + dx[t], y1 = y + dy[t];
			if(x1 < 0 || y1 < 0 || x1 >= r || y1 >= c)
				continue;
			if(col[x1][y1] != -1){
				if(col[x1][y1] > b+d){
					cout << "IMPOSSIBLE" << endl;
					return;
				}
			} else {
				col[x1][y1] = b+d;
				ko.push({-b-d, {x1,y1}});
			}
		}
	}
	ll ans = 0;
	trav(v, col) trav(x, v) ans = (ans + x)%mod;
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