#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <cstring>
#include <cmath>
using namespace std;

#define Rep(i,n) for(int i=0;i<(n);++i)
#define For(i,a,b) for(int i=(a);i<=(b);++i)
#define Ford(i,a,b) for(int i=(a);i>=(b);--i)
#define fi first
#define se second
#define pb push_back
#define MP make_pair

#define sqr(x) ((x)*(x))

typedef pair<int,int> PII;
typedef vector<int> VI;

char a[222][222];
int n;

bool digit(char c){
	return '0'<=c&&c<='9';	
}

int num(int i){
	if(i<=n)return n-i+2*i-1;
	else return i-n+2*(2*n-i)-1;
}

int main() {
	int st;
	cin>>st;
	Rep(tt,st){
		cerr<<tt+1<<"/"<<st<<endl;
		scanf("%d",&n);
		memset(a,0,sizeof(a));
		gets(a[0]);
		For(i,1,2*n-1)
			gets(a[i]+1);
		int best=1000000000;
		For(i,-2*n,4*n){
			For(j,-2*n,4*n){
				int count=0;
				bool can=true;
				int maxdistance=0;
				For(u,1,2*n-1){
					int sl=num(u);
					For(v,1,sl)if(digit(a[u][v])){
						maxdistance=max(maxdistance,abs(u-i)+abs(v-j));
						++count;
						For(h,1,3){
							int x,y;
							if(h==1)x=2*i-u,y=2*j-v;
							if(h==2)x=2*i-u,y=v;
							if(h==3)x=u,y=2*j-v;
							bool ok=true;
							if(1<=x&&x<2*n){
								int sl2=num(x);
								if(y<=sl2&&y>0&&digit(a[x][y])&&a[x][y]!=a[u][v])ok=false;
							}
							if(!ok){can=false;break;}
						}
						if(!can)break;
					}
					if(!can)break;
				}	
				if(can){
					maxdistance++;
					int cost=sqr(maxdistance)-count;
					if(best>cost)best=cost;
				}
			}	
		}
		cout<<"Case #"<<tt+1<<": ";	
		cout<<best<<endl;
	}	
	return 0;	
}
