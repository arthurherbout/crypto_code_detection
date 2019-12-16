#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <cassert>
#include <iostream>
using namespace std;
#define rep(i,a,n) for (int i=a;i<n;i++)
#define per(i,a,n) for (int i=n-1;i>=a;i--)
#define pb push_back
#define mp make_pair
#define all(x) (x).begin(),(x).end()
#define fi first
#define se second
#define SZ(x) ((int)(x).size())
typedef vector<int> VI;
typedef long long ll;
typedef pair<int,int> PII;
const ll mod=1000000007;
ll powmod(ll a,ll b) {ll res=1;a%=mod; assert(b>=0); for(;b;b>>=1){if(b&1)res=res*a%mod;a=a*a%mod;}return res;}
// head



typedef long double db;
db sqr(db x){ return x*x; }
const db EPS = 1e-9,pi=acosl(-1.);
  
inline int sign(db a) { return a < -EPS ? -1 : a > EPS; }
  
inline int cmp(db a, db b){ return sign(a-b); }

struct P3{
    db x,y,z;
    P3 operator+(P3 o){ return {x+o.x,y+o.y,z+o.z}; }
    P3 operator-(P3 o){ return {x-o.x,y-o.y,z-o.z}; }
    db operator*(P3 o){ return x*o.x+y*o.y+z*o.z; }
    P3 operator^(P3 o){ return {y*o.z-z*o.y,z*o.x-x*o.z,x*o.y-y*o.x}; }
    P3 operator*(db o){ return {x*o,y*o,z*o}; }
    P3 operator/(db o){ return {x/o,y/o,z/o}; }

    db abs2(){ return sqr(x) + sqr(y) + sqr(z); }
    db abs(){ return sqrt(abs2()); }

    P3 norm(){ return *this / abs(); } 
    bool operator<(P3 o){
        if(cmp(x,o.x) != 0) return x < o.x;
        if(cmp(y,o.y) != 0) return y < o.y;
        return cmp(z,o.z) == -1;
    }
    bool operator==(P3 o){
        return cmp(x,o.x) == 0 && cmp(y,o.y) == 0 && cmp(z,o.z) == 0;
    }
    void read(bool f){
        cin>>x>>y>>z;
        if (f) cout<<x<<" "<<y<<" "<<z<<endl;
    }
    void print(){
        //printf("%lf,%lf,%lf\n",x,y,z);
    }
};

typedef vector<P3> VP;
typedef vector<VP> VVP;

db r;

db Acos(db x) {
    return acos(max(-(db)1,min(x,(db)1)));
}

db dist(P3 a,P3 b){// qiumian juli
    db r=Acos(a*b);
    return r;
}

vector<db> solve(db a,db b,db c) {
    // return cos(t)*a+sin(t)*b < c
    // a=r*cos(th) b=r*sin(th)
    db r=sqrt(a*a+b*b);
    db th=atan2(b,a);
    // r*cos(t-th) <= c
    if (cmp(c,-r)<=0)  return {0}; // c <= -r
    else if (cmp(r,c) == -1) return {1}; // r < c
    else {
        db tr=pi-Acos(c/r);     
//        assert(tr < pi);
        return {th+pi-tr+(1e-12),th+pi+tr-(1e-12)};
    }
}

P3 rnd,p[5050];

vector<db> jiao(P3 a,P3 b){
    if (cmp(dist(a,b),2*r)>=0) return {0};
    if (cmp(dist(a,b),0)<=0) return {1}; 
    P3 rd=a*cos(r); P3 z=a.norm(); P3 y=(z^rnd).norm(); P3 x=(y^z).norm();
    // (rd+x*cos(t)+y*sin(t))*b >= cos(r)
    vector<db> ret = solve(-(x*b*sin(r)),-(y*b*sin(r)),-(cos(r)-rd*b)); 
    //return solve(-(x*b*sin(r)),-(y*b*sin(r)),-(cos(r)-rd*b));
    return ret;
}

db norm(db x){ //[0,2pi)
    while(x < 0) x+= 2*pi;
    while(x >= 2*pi) x-= 2*pi;
    return x;
}
int n;

bool fg;
bool solve() {
	scanf("%d",&n);
	rep(i,0,n) {
		p[i].read(fg);
		p[i]=p[i].norm();
	}
	r=pi/2;
	rep(k,0,n) {
		rnd=(P3){(db)rand(),(db)rand(),(db)rand()}.norm();		
		int cnt=0;
		vector<pair<db,int> > rr;
		rep(i,0,n) if (i!=k) {
			auto c=jiao(p[k],p[i]);
			if (SZ(c)==1) {
				if (c[0]==0) return 1;
				if (c[0]==1) cnt++;
			} else {
				db l=norm(c[0]),r=norm(c[1]);
				rr.pb(mp(l,1)),rr.pb(mp(r,-1));
				if (l>r) cnt++;			
			}
		}
		if (cnt==n-1) return 0;
		sort(all(rr));
		rep(i,0,SZ(rr)) {
			cnt+=rr[i].se;
			if (cnt==n-1) return 0;
		}
	}
	return 1;
}

int _,__;
int main() {
	for (scanf("%d",&_);_;_--) {
		printf("Case #%d: ",++__);
		fprintf(stderr,"Case #%d\n",__);
		puts(solve()?"YES":"NO");
	}
}
