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

int dx[] = {1,0,-1,0}, dy[] = {0,1,0,-1};
	
void solve(){
	int c,r,m;
	cin >> c >> r >> m;
	vector<string> grid(r);
	trav(s, grid) cin >> s;

	vector<vi> num(r, vi(c));
	int ix = 0;
	rep(i,0,r) rep(j,0,c) if(grid[i][j]=='S'){
		num[i][j] = ix++;
	}
	int ns = ix;
	ix = 0;
	vector<vector<vi>> turs(r, vector<vi>(c));

	rep(i,0,r) rep(j,0,c) if(grid[i][j]=='T'){
		num[i][j] = ix;
		for(int x = i-1; x >= 0 && grid[x][j]!='#'; --x)
			turs[x][j].push_back(ix);
		for(int x = i+1; x < r && grid[x][j]!='#'; ++x)
			turs[x][j].push_back(ix);
		for(int x = j-1; x >= 0 && grid[i][x]!='#'; --x)
			turs[i][x].push_back(ix);
		for(int x = j+1; x < c && grid[i][x]!='#'; ++x)
			turs[i][x].push_back(ix);

		++ix;
	}
	int nt = ix;

	vector<vi> ls(ns);

	rep(i,0,r) rep(j,0,c) if(grid[i][j]=='S'){
		vi reach(nt, m+1);
		vector<vi> dist(r,vi(c,-1));
		queue<pii> ko;
		ko.push({i,j});
		dist[i][j] = 0;
		while(!ko.empty()){
			int x = ko.front().first, y = ko.front().second;
			ko.pop();

			trav(t, turs[x][y]) reach[t] = min(reach[t], dist[x][y]);
			if(dist[i][j]==m) continue;
			rep(t,0,4){
				int x1 = x+dx[t], y1 = y+dy[t];
				if(x1>=0&&y1>=0&&x1<r&&y1<c&
					grid[x1][y1]!='#'&&dist[x1][y1]==-1){
					
					dist[x1][y1] = dist[x][y]+1;
					ko.push({x1,y1});
				}
			}
		}
		ls[num[i][j]] = reach;
	}

	vector<vector<bool>> vis(1<<ns, vector<bool>(1<<nt));

	int y = 0;
	vector<pii> ans;
	function<bool(int,int,bool)> rek = [&](int s, int t, bool go){
		if(vis[s][t]) return 0;
		vis[s][t] = 1;
		if(go && y==nt-__builtin_popcount(t)) return 1;
		y = max(y, nt-__builtin_popcount(t));
		rep(i,0,ns) if((s>>i)&1){
			int lo = m+1;
			rep(j,0,nt) if((t>>j)&1) lo = min(lo, ls[i][j]);
			if(lo>m) continue;
			rep(j,0,nt) if(((t>>j)&1) && lo==ls[i][j]){
				if(rek(s-(1<<i), t-(1<<j), go)){
					ans.emplace_back(i+1,j+1);
					return 1;
				}
			} 
		}
		return 0;
	};
	rek((1<<ns)-1, (1<<nt)-1, 0);
	rep(i,0,1<<ns) rep(j,0,1<<nt) vis[i][j] = 0;
	rek((1<<ns)-1, (1<<nt)-1, 1);
	reverse(all(ans));
	cout << y << endl;
	trav(pa, ans) cout << pa.first << ' ' << pa.second << endl;

}

int main(){
	int n;
	cin >> n;
	rep(t,1,n+1){
		cerr << t << endl;
		cout << "Case #" << t << ": ";
		solve();
	}
}