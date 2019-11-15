#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <queue>
#include <string>
#include <cstring>
#include <stack>
using namespace std;
#define rep(i,a,b) for(int i = (a);i<=(b);i++)
#define repk(i,a,b) rep(i,(a),(b)-1)
#define rrep(i,a,b) for(int i = (b);i>=(a);i--)
#define rrepk(i,a,b) rrep(i,(a),(b)-1)
const int inf = 0x3fffffff,dinf = 0x7fffffff,geps = 10;
typedef long long ll;

const int maxn = 3000;

struct vec{
	ll x,y;
	vec() : x(0), y(0){}
	vec(ll _x, ll _y) : x(_x), y(_y){}
	ll operator*(const vec& r) const{
		return x * r.x + y * r.y;
	}
	ll operator^(const vec& r) const{
		return x * r.y - r.x * y;
	}
	vec operator-(const vec& r) const{
		return vec(x - r.x, y - r.y);
	}
	vec operator+(const vec& r) const{
		return vec(x + r.x, y + r.y);
	}
	int sgn() const{
		if(y < 0) return -1;
		else if(y > 0) return 1;
		else return x < 0 ? -1 : 1;
	}
	bool operator<(const vec& r) const{
		if(sgn() != r.sgn()){
			return sgn() > r.sgn();
		}else{
			return operator^(r) > 0;
		}
	}
	bool operator||(const vec& r) const{
		return operator^(r) == 0;
	}
};
vector<vec> vecs;

void scanf(vec* r){
	scanf("%lld%lld",&r->x, &r->y);
}

int n;
void Init(){
	vecs.clear();
	scanf("%d",&n);
	vec x;
	repk(i,0,n){
		scanf(&x);
		vecs.push_back(x);
	}

	// for(vector<vec>::iterator it = vecs.begin(); it != vecs.end(); it++)
		// printf("vecs: %lld %lld\n", it->x, it->y);
}
vector<vec> V;
int calc(int ind){
	V.clear();
	repk(i,0,n) if(i != ind){
		V.push_back(vecs[i] - vecs[ind]);
	}
	// for(vector<vec>::iterator it = V.begin(); it != V.end(); it++)
		// printf("Vold: %lld %lld\n", it->x, it->y);
	sort(V.begin(), V.end());

	// for(vector<vec>::iterator it = V.begin(); it != V.end(); it++)
		// printf("Vpre: %lld %lld\n", it->x, it->y);

	int size = V.size();
	repk(i,0,size) V.push_back(V[i]);
	int pr = 0, ans = 0;
	repk(i,0,size){
		while(pr < i + size && (V[pr] ^ V[i]) <= 0) pr++;
		// printf("pr: %d\n",pr);
		ans = max(ans, pr - i);
	}
	// printf("ans: %d\n",ans);
	return n - ans - 1;
}
void solve(){
	repk(i,0,n){
		printf("%d\n",calc(i));
	}
}

int main(){

	int Test;
	scanf("%d",&Test);
	for(int t = 0; t < Test; ++t){
		Init();
		printf("Case #%d:\n", t+1);
		solve();
	}
	
	return 0;
}
