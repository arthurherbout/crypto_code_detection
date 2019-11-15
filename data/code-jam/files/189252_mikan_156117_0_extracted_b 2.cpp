#include<cstdio>
#include<iostream>
#include<algorithm>
#include<cstring>
#include<cstdlib>
#include<cmath>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<queue>
#include<bitset>
#include<cassert>

using namespace std;

typedef long long ll;
typedef pair<int,int> pint;
typedef vector<int> vint;

#define mp make_pair
#define pb push_back
#define REP(i,a,b) for(int i=a;i<b;++i)
#define rep(i,n) REP(i,0,n)

#define EPS 1e-11
inline bool iszero(double a){return -EPS<a && a<EPS;}

int t;
int n;
double X,Y,Z,mx,my,mz;

inline double pw(double a){return a*a;}

int main(){
	freopen("b.in","r",stdin);
	freopen("b.out","w",stdout);
	cin>>t;
	REP(cas,1,t+1){
		cin>>n;
		X=Y=Z=mx=my=mz=0.0;
		rep(i,n){
			int x,y,z,vx,vy,vz;
			scanf("%d%d%d%d%d%d",&x,&y,&z,&vx,&vy,&vz);
			X+=x; Y+=y; Z+=z;
			mx+=vx;my+=vy;mz+=vz;
		}
		X/=n;Y/=n;Z/=n;mx/=n;my/=n;mz/=n;
		double ns=X*mx+Y*my+Z*mz;
		if(iszero(pw(mx)+pw(my)+pw(mz))){
//			cout<<"type1"<<endl;
			if(pw(X)+pw(Y)+pw(Z)<EPS)
				printf("Case #%d: %f %f\n",cas,0.0,0.0);
			else
				printf("Case #%d: %f %f\n",cas,sqrt(pw(X)+pw(Y)+pw(Z)),0.0);
			continue;
		}
		if(ns>=0){
//			cout<<"type2"<<endl;
			if(pw(X)+pw(Y)+pw(Z)<EPS)
				printf("Case #%d: %f %f\n",cas,0.0,0.0);
			else
				printf("Case #%d: %f %f\n",cas,sqrt(pw(X)+pw(Y)+pw(Z)),0.0);
		}else{
//			cout<<"type3"<<endl;
			double b=sqrt(pw(mx)+pw(my)+pw(mz));
			double ac=-ns/b;
			double a2=pw(X)+pw(Y)+pw(Z);
			double as= a2-pw(ac)>EPS ? sqrt(a2-pw(ac)) : 0.0;
			printf("Case #%d: %f %f\n",cas,as,ac/b);
		}
	}
	return 0;
}
