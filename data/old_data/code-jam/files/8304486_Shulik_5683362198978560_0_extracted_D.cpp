#include <bits/stdc++.h>

#define REP(i, n) for(int i = 0; i < n; i++)

#define MOD 1000000007
#define X first
#define Y second
#define mp make_pair

using namespace std;

typedef long double ld;
typedef long long ll;
typedef pair<int, int> ii;

int r, c, n;
ll d;

ll br[234][234];
bool done[234][234];
priority_queue<pair<ll, ii> > pq;
ii dd[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

bool valid(){
	REP(x, r)REP(y, c){
		assert(done[x][y]);
		ll val = br[x][y];
		for(auto ddd : dd){
			int nx = x+ddd.first, ny = y+ddd.second;
			if(nx < 0 || ny < 0 || nx >= r || ny >= c)continue;
			if(abs(val - br[nx][ny]) > d)return false;
		}
	}
	return true;
}

void testcase(int tcn){

	cin >> r >> c >> n >> d;

	REP(i, r)REP(j, c)done[i][j] = false;

	int x, y;
	ll b;
	REP(i, n){
		cin >> x >> y >> b;
		--x;--y;
		br[x][y] = b;
		done[x][y] = true;

		pq.push(mp(-b, mp(x, y)));
	}

	while(!pq.empty()){
		auto tt = pq.top();
		pq.pop();
		x = tt.Y.X, y = tt.Y.Y;
		ll val = br[x][y];
		//cerr << "doing " << x << " " << y << ": " << val << endl;
		for(auto ddd : dd){
			int nx = x+ddd.first, ny = y+ddd.second;
			if(nx < 0 || ny < 0 || nx >= r || ny >= c)continue;
			if(done[nx][ny])continue;
			br[nx][ny] = val+d;
			done[nx][ny] = true;
			pq.push(mp(-(val+d), mp(nx, ny)));
			//cerr << "      updated " << nx << " " << ny << ": " << br[nx][ny] << endl;
		}
	}

	if(!valid()){
		cout << "Case #"<< tcn<<": IMPOSSIBLE"<<endl;
	} else{
		ll cnt = 0;
		REP(i, r)REP(j, c){
			cnt = (cnt+br[i][j])%MOD;
		}
		cout << "Case #"<< tcn<<": "<<cnt<<endl;
	}

}

int main(){

	int T;
	cin >> T;
	REP(i, T){
		testcase(i+1);
	}

	return 0;

}