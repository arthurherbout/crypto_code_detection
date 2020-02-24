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

const int mx = 10;

int pw[mx][mx];

int dec(int x, int l){
	int y = 0;
	for(; x; x /= (l+1))
		y += pw[l][l - x%(l+1)];
	return y;
}

int toi(string s){
	int x = 0;
	trav(c, s){
		x = (sz(s)+1)*x + c-'0';
	}
	return x;
}

int main(){
	rep(l,1,mx){
		pw[l][0] = 1;
		rep(i,1,l) pw[l][i] = pw[l][i-1]*(l+1);
		pw[l][l] = 0;
	}

	vector<vi> ans(mx);

	rep(l,1,mx){
		int num = 1;
		rep(_,0,l) num *= l+1;
		ans[l].resize(num,1);
		vi pek(num);
		rep(x,1,num){
			pek[x] = dec(x,l);
			if(pek[x] == x) pek[x] = 0;
		}
		vi deg(num);
		rep(x,1,num) ++deg[pek[x]];
		stack<int> lf;
		rep(x,1,num) if(deg[x] == 0) lf.push(x);
		while(!lf.empty()){
			int x = lf.top(); lf.pop();
			ans[l][pek[x]] += ans[l][x];
			if(--deg[pek[x]] == 0) lf.push(pek[x]);
		}
		rep(x,1,num) assert(deg[x] == 0);
	}
	int n;
	cin >> n;
	rep(t,1,n+1){
		cout << "Case #" << t << ": ";
		string g;
		cin >> g;
		cout << ans[sz(g)][toi(g)] << endl;
	}
}