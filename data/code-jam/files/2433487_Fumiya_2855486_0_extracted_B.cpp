#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <functional>
#include <complex>
#include <vector>

using namespace std;

const double EPS = 1e-8;
typedef complex<double> P;
typedef vector<P> G;

struct L { P p, q; L(P p, P q) : p(p), q(q) {} };

double dot(P a, P b) { return real(conj(a)*b); }
double cross(P a, P b) { return imag(conj(a)*b); }
double angle(P a, P b) { return arg(conj(a)*b); }

bool compare(const P &a, const P &b){
	return make_pair(real(a),imag(a)) < make_pair(real(b),imag(b));
}

double polygonArea(G g){
	double res = 0.0;
	for(int i=0;i<g.size();i++)
		res += cross(g[i],g[(i+1)%g.size()]);
	return abs(0.5*res);
}

G convexHull(G vp){
	if(vp.size() < 3) return vp;
	sort(vp.begin(), vp.end(), compare);
	vector< pair<double,int> > vi;
	for(int i=1;i<vp.size();i++) vi.push_back(make_pair(angle(P(1,0),vp[i]-vp[0]), i));
	sort(vi.begin(), vi.end());
	G res(2);
	res[0] = vp[0]; res[1] = vp[vi[0].second];
	for(int i=1;i<vi.size();i++){
		P next = vp[vi[i].second];
		int sz = res.size();
		while(sz>=2&&cross(res[sz-1]-res[sz-2],next-res[sz-1])<EPS){
			res.pop_back(); sz--;
		}
		res.push_back(next);
	}
	return res;
}

bool ssIntersect(L a, L b){
	if(abs(imag((a.q-a.p)/(b.q-b.p)))<EPS) return false;
	return cross(a.q-a.p, b.p-a.p)*cross(a.q-a.p, b.q-a.p) < EPS &&
    	   cross(b.q-b.p, a.p-b.p)*cross(b.q-b.p, a.q-b.p) < EPS;
}

bool spIntersect(L l, P p){
	return abs(abs(l.p-p)+abs(l.q-p)-abs(l.p-l.q)) < EPS;
}

vector<int> solveSmall(const G& g){
	int n = g.size();
	double area = polygonArea(convexHull(g));
	vector<int> res(g.size());
	for(int i=0;i<g.size();i++) res[i] = i;
	G copy = g;
	do{
		bool ok = true;
		for(int i=0;i<n;i++){
			for(int j=i+1;j<n;j++){
				if((i+1)%n == j || (j+1)%n == i) continue;
				if(ssIntersect(L(g[res[i]], g[res[(i+1)%n]]), L(g[res[j]], g[res[(j+1)%n]]))) ok = false; 
			}
			for(int j=0;j<n;j++){
				if(i==j || (i+1)%n==j) continue;
				if(spIntersect(L(g[res[i]], g[res[(i+1)%n]]), g[res[j]])) ok = false;
			}
		}
		if(!ok) continue;
		for(int i=0;i<n;i++) copy[i] = g[res[i]];
		if(polygonArea(copy) > 0.5*area+EPS) return res;
	}while(next_permutation(res.begin(), res.end()));
	return vector<int>();
}

int main(){
	int TEST; cin >> TEST;
	static int lower[10000], upper[10000], used[10000];
	for(int test=1;test<=TEST;test++){
		int N; cin >> N;
		G g(N);
		for(int i=0;i<N;i++) cin >> g[i].real() >> g[i].imag();
		vector<int> res = solveSmall(g);
		printf("Case #%d:", test);
		for(int i=0;i<res.size();i++) cout << " " << res[i];
		cout << endl;
	}
}
