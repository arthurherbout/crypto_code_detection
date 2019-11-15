#include<cstdio>
#include<iostream>
#include<sstream>
#include<algorithm>
#include<vector>
#include<string>
#include<queue>
#include<set>
#include<map>
#include<cstring>
#include<cstdlib>
//#include<cmath>
//#include<gmpxx.h>

using namespace std;

typedef pair<int,int> pint;
typedef vector<int> vint;
typedef long long ll;
//typedef mpz_class Bigint;

#define repp(i,a,b) for(int i=(int)(a);i<=(int)(b);++i)
#define rep(i,n) repp(i,0,n-1)

int t,k;

int in(){
	int n;
	scanf("%d",&n);
	return n;
}


bool check(int x,int y,vector<vint> &d,vector<vint> &u){
	repp(xx,1,x-1){
		repp(yy,1,2*k-1){
			if(u[xx][yy] && u[2*x-xx][yy] && d[xx][yy]!=d[2*x-xx][yy])return 0;
		}
	}
	repp(yy,1,y-1){
		repp(xx,1,2*k-1){
			if(u[xx][yy] && u[xx][2*y-yy] && d[xx][yy]!=d[xx][2*y-yy])return 0;
		}
	}
//	cout<<x<<" "<<y<<endl;
	return 1;
}

#define pw(x) ((x)*(x))
int main(){
	cin>>t;
//	cout<<t<<endl;
	repp(cas,1,t){
		cin>>k;
//		cout<<cas<<" "<<t<<endl;
		vector<vint> d(400,vint(400,-1)),u(400,vint(400,0));
		repp(i,1,k)rep(j,i)d[k+2*j-i+1][i]=in(),u[k+2*j-i+1][i]=1;
		for(int i=k-1;i>0;--i)rep(j,i)d[k+2*j-i+1][2*k-i]=in(),u[k+2*j-i+1][2*k-i]=1;
		int ans=1001001001;
/*
		repp(y,1,2*k-1){
			repp(x,1,2*k-1){
				if(u[x][y])cout<<d[x][y];
				else cout<<" ";
			}
			cout<<endl;
		}
*/
		rep(i,k){
			rep(j,k){
				if(i+j>=ans)continue;
				if(check(k-j,k-i,d,u))ans=i+j;
				if(check(k+j,k-i,d,u))ans=i+j;
				if(check(k-j,k+i,d,u))ans=i+j;
				if(check(k+j,k+i,d,u))ans=i+j;
			}
		}
		printf("Case #%d: %d\n",cas,pw(ans+k)-pw(k));
	}
	return 0;
}
