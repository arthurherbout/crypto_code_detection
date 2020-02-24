#include <bits/stdc++.h>
#define F first
#define S second
#define X real()
#define Y imag()
using namespace std;
typedef long long ll;
typedef long double ld;

int legalx(const vector<pair<int, int> >& c){
	for (int i=0;i<(int)c.size();i++){
		for (int ii=i+1;ii<(int)c.size();ii++){
			if (c[i].F==c[ii].F) return 0;
			if (c[i].S==c[ii].S) return 0;
		}
	}
	return 1;
}

int legalp(const vector<pair<int, int> >& c){
	for (int i=0;i<(int)c.size();i++){
		for (int ii=i+1;ii<(int)c.size();ii++){
			if (c[i].F+c[i].S==c[ii].F+c[ii].S) return 0;
			if (c[i].F-c[i].S==c[ii].F-c[ii].S) return 0;
		}
	}
	return 1;
}

struct MaxFlow {
	struct e {
		int to,r;
		ll f;
		pair<int, int> id;
	};
	vector<vector<e> > g;
	vector<int> used;
	int cc;
	ll flow(int x, int t, ll fl, ll miv) {
		if (x==t) return fl;
		used[x]=cc;
		for (auto& nx:g[x]) {
			if (used[nx.to]!=cc&&nx.f>=miv) {
				ll r=flow(nx.to, t, min(fl, nx.f), miv);
				if (r>0) {
					nx.f-=r;g[nx.to][nx.r].f+=r;
					return r;
				}
			}
		}
		return 0;
	}
	// maxv is maximum expected maxflow
	ll getMaxFlow(int source, int sink, ll maxv) {
		cc=1;ll r=0;ll k=1;
		while (k*2<=maxv) k*=2;
		for (;k>0;k/=2) {
			while (ll t=flow(source, sink, maxv, k)) {
				r+=t;cc++;
			}
			cc++;
		}
		return r;
	}
	void addEdge(int a, int b, ll c, pair<int, int> id) {
		g[a].push_back({b, (int)g[b].size(), c, {-1, -1}});
		g[b].push_back({a, (int)g[a].size()-1, 0, id});
	}
	MaxFlow(int n) : g(n+1), used(n+1) {}
};

vector<pair<int, int> > solvep(int n, const vector<pair<int, int> >& ps){
	vector<int> u1(2*n-1);
	vector<int> u2(2*n-1);
	for (auto p:ps){
		int d1=p.F+p.S;
		int d2=p.F-p.S+n-1;
		assert(0<=d1&&d1<2*n-1);
		assert(0<=d2&&d2<2*n-1);
		u1[d1]=1;
		u2[d2]=1;
	}
	MaxFlow mf(4*n+10);
	int so=4*n+5;
	int si=4*n+6;
	for (int i=0;i<2*n-1;i++){
		if (!u1[i]) mf.addEdge(so, i, 1, {-1, -1});
		if (!u2[i]) mf.addEdge(2*n+i, si, 1, {-1, -1});
	}
	for (int i=0;i<n;i++){
		for (int ii=0;ii<n;ii++){
			int d1=i+ii;
			int d2=i-ii+n-1;
			mf.addEdge(d1, 2*n+d2, 1, {i, ii});
		}
	}
	int fl=mf.getMaxFlow(so, si, 1);
	vector<pair<int, int> > ans;
	for (int i=0;i<4*n+10;i++){
		for (auto nx:mf.g[i]){
			if (nx.f==1&&nx.id.F>=0){
				ans.push_back(nx.id);
			}
		}
	}
	return ans;
}

vector<pair<int, int> > solvex(int n, const vector<pair<int, int> >& xs){
	vector<int> u1(n);
	vector<int> u2(n);
	for (auto p:xs){
		assert(0<=p.F&&p.F<n);
		assert(0<=p.S&&p.S<n);
		u1[p.F]=1;
		u2[p.S]=1;
	}
	MaxFlow mf(2*n+10);
	int so=2*n+5;
	int si=2*n+6;
	for (int i=0;i<n;i++){
		if (!u1[i]) mf.addEdge(so, i, 1, {-1, -1});
		if (!u2[i]) mf.addEdge(n+i, si, 1, {-1, -1});
	}
	for (int i=0;i<n;i++){
		for (int ii=0;ii<n;ii++){
			mf.addEdge(i, n+ii, 1, {i, ii});
		}
	}
	int fl=mf.getMaxFlow(so, si, 1);
	vector<pair<int, int> > ans;
	for (int i=0;i<2*n+10;i++){
		for (auto nx:mf.g[i]){
			if (nx.f==1&&nx.id.F>=0){
				ans.push_back(nx.id);
			}
		}
	}
	return ans;
}

void solve(){
	vector<pair<int, int> > ps;
	vector<pair<int, int> > xs;
	int n,m;
	cin>>n>>m;
	for (int i=0;i<m;i++){
		char t;
		int x,y;
		cin>>t>>x>>y;
		x--;
		y--;
		if (t=='+'||t=='o') ps.push_back({x, y});
		if (t=='x'||t=='o') xs.push_back({x, y});
	}
	assert(legalx(xs));
	assert(legalp(ps));
	
	auto nps=solvep(n, ps);
	auto nxs=solvex(n, xs);
	
	vector<pair<int, int> > p=ps;
	p.insert(p.end(), nps.begin(), nps.end());
	assert(legalp(p));
	vector<pair<int, int> > x=xs;
	x.insert(x.end(), nxs.begin(), nxs.end());
	assert(legalx(x));
	
	vector<pair<int, int> > no;
	vector<pair<int, int> > np;
	vector<pair<int, int> > nx;
	
	for (auto pp:nps){
		int fo=0;
		for (auto xx:x){
			if (xx==pp) fo=1;
		}
		if (fo) no.push_back(pp);
		else np.push_back(pp);
	}
	for (auto xx:nxs){
		int fo=0;
		for (auto pp:p){
			if (xx==pp) fo=1;
		}
		for (auto pp:ps){
			if (xx==pp){
				fo=2;
			}
		}
		if (fo==2){
			no.push_back(xx);
		}
		else if(fo==0){
			nx.push_back(xx);
		}
	}
	
	cout<<p.size()+x.size()<<" "<<no.size()+np.size()+nx.size()<<endl;
	for (auto xx:nx){
		cout<<"x "<<xx.F+1<<" "<<xx.S+1<<endl;
	}
	for (auto xx:np){
		cout<<"+ "<<xx.F+1<<" "<<xx.S+1<<endl;
	}
	for (auto xx:no){
		cout<<"o "<<xx.F+1<<" "<<xx.S+1<<endl;
	}
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