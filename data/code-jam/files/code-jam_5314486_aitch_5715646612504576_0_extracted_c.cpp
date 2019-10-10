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
	int r,c;
	cin >> r >> c;
	vector<string> grid(r);
	trav(s, grid) cin >> s;
	int ix = 0;
	vector<vector<vi>> ls(r, vector<vi>(c));

	function<bool(int,int,int,bool)> go =
		[&](int x, int y, int t, bool b){

		while(true){
			x += dx[t], y += dy[t];
			if(x<0||y<0||x>=r||y>=c) return 1;
			else if(grid[x][y]=='#') return 1;
			else if(grid[x][y]=='-'||grid[x][y]=='|') return 0;
			else if(grid[x][y]=='/') t = (3-t)%4;
			else if(grid[x][y]=='\\') t = (5-t)%4;
			else if(b) ls[x][y].push_back(ix);
		}
	};


	vector<pii> pos;

	rep(i,0,r) rep(j,0,c) if(grid[i][j]=='|'||grid[i][j]=='-'){
		pos.push_back({i,j});
		++ix;
		rep(t,0,2){
			if(go(i,j,0+t,0) && go(i,j,2+t,0)){
				go(i,j,0+t,1);
				go(i,j,2+t,1);
			} else {
				ls[i][j].push_back(-ix);
			}
			ix = -ix;
		}
		if(sz(ls[i][j])==2){
			puts("IMPOSSIBLE");
			return;
		}
	}
	ix = sz(pos);
	int m = 2*ix;
	vector<vector<bool>> better(m, vector<bool>(m));
	rep(i,0,m) better[i][i] = 1;

	auto neg = [&](int x){
		return x<ix ? x+ix : x-ix;
	};


	rep(i,0,r) rep(j,0,c) if(sz(ls[i][j]) || grid[i][j]=='.'){
		if(sz(ls[i][j])==0){
			cout << "IMPOSSIBLE" << endl;
			return;
		}
		assert(sz(ls[i][j]) <= 2);
		trav(x, ls[i][j]){
			if(x>0) --x;
			else {
				x = -x;
				--x;
				x += ix;
			}
		}
		int a = ls[i][j][0], b = a;
		if(sz(ls[i][j])==2){
			b = ls[i][j][1];
			better[neg(b)][a] = 1;
		}
		better[neg(a)][b] = 1;
	}
	rep(k,0,m) rep(i,0,m) rep(j,0,m)
		if(better[i][k] && better[k][j])
			better[i][j] = 1;
	rep(i,0,ix) if(better[i][neg(i)] && better[neg(i)][i]){
		puts("IMPOSSIBLE");
		return;
	}
	vi val(m, -1);
	while(count(all(val), -1)){
		rep(i,0,m) if(val[i]==-1){
			rep(j,0,m){
				if(val[j]==-1 && better[i][j] && !better[j][i])
					goto bad;
				if(val[j]==0 && better[i][j])
					goto bad;
			}
			val[i] = 1;
			val[neg(i)] = 0;

			break;
			bad: ;
		}
	}
	rep(i,0,ix){
		int x = pos[i].first, y = pos[i].second;
		grid[x][y] = val[i] ? '|' : '-';
	}
	cout << "POSSIBLE" << endl;
	trav(s, grid) cout << s << endl;
}

int main(){
	int n;
	cin >> n;
	rep(t,1,n+1){
		cout << "Case #" << t << ": ";
		solve();
	}
}