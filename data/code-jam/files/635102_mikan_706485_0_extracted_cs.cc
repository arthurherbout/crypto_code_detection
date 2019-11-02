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

#define pb push_back
#define mp make_pair
#define repp(i,a,b) for(int i=(int)(a);i<=(int)(b);++i)
#define rep(i,n) repp(i,0,(n)-1)

int c,r;

int in(){
	int nnnn;scanf("%d",&nnnn);return nnnn;
}

int main(){
	cin>>c;
	repp(cas,1,c){
		cin>>r;
		vector<vint> m(200,vint(200,0));
		rep(i,r){
			int x1,x2,y1,y2;
			cin>>x1>>y1>>x2>>y2;
			repp(x,x1,x2)repp(y,y1,y2){
				m[x][y]=1;
			}
		}
		int ans=-1;
		bool frag=1;
		for(;frag;++ans){
			frag=0;
			for(int x=100;x;--x)for(int y=100;y;--y){
				if(m[x][y]){
					frag=1;
					if(!m[x][y-1] && !m[x-1][y])m[x][y]=0;
				}else{
					if(m[x][y-1] && m[x-1][y])m[x][y]=1;
				}
			}
		}
		printf("Case #%d: %d\n",cas,ans);
	}
	return 0;
}
