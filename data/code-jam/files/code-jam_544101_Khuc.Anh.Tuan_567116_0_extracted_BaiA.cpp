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

int dx[8] = {-1,-1,-1,0,1,1,1,0};
int dy[8] = {-1,0,1,1,1,0,-1,-1};

int n,k;
char a[55][55], b[55][55];

int main() {
	int st;
	scanf("%d",&st);
	Rep(t,st){
		scanf("%d%d",&n,&k);	
		gets(a[0]);
		Rep(i,n)gets(a[i]);
		memset(b,'.',sizeof(b));
		Rep(i,n)Rep(j,n)if(a[i][j]!='.')b[j][n-1-i]=a[i][j];
		Rep(j,n){
			int s=n-1;
			Ford(i,n-1,0){
				if(b[i][j]!='.'){
					char c=b[i][j];
					b[i][j]='.';
					b[s--][j]=c;	
				}	
			}
		}
		bool red=false,blue=false;
		Rep(i,n)Rep(j,n)if(b[i][j]!='.'){
			Rep(h,8){
				bool ok=true;
				Rep(v,k){
					int x=dx[h]*v+i;
					int y=dy[h]*v+j;
					if(x<0||x>=n||y<0||y>=n){ok=false;break;}
					if(b[x][y]!=b[i][j]){ok=false;break;}
				}
				if(ok){
					if(b[i][j]=='R')red=true;
					if(b[i][j]=='B')blue=true;	
				}
			}	
		}
		cout << "Case #" << t+1 << ": ";
		if(red&&blue)cout<<"Both";
		else if(red)cout<<"Red";
		else if(blue)cout<<"Blue";
		else cout<<"Neither";
		cout<<endl;
	}
	return 0;	
}
