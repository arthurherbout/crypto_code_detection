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

int D,I,M,n,inf;
int a[111];
int F[111][256];

int main() {
	int st;
	scanf("%d",&st);
	Rep(t,st){
		//cout<<"test="<<t+1<<endl;
		scanf("%d%d%d%d",&D,&I,&M,&n);
		For(i,1,n)scanf("%d",a+i);
		memset(F,0x1f,sizeof(F));
		inf=F[0][0];
		Rep(i,256)F[0][i]=0;
		Rep(i,n){
			//cout<<"i="<<i<<endl;
			// insert
			Rep(j,256)if(F[i][j]<inf){
				Rep(k,256)if(k!=j){
					if(M>0){
						int need=(abs(j-k)-1)/M+1;
						if(F[i][k]>F[i][j]+need*I)
							F[i][k]=F[i][j]+need*I;
					}
				}	
			}	
			//cout<<"here"<<endl;
			Rep(j,256)if(F[i][j]<inf){
				// del
				F[i+1][j]<?=F[i][j]+D;
				// keep
				if(abs(a[i+1]-j)<=M) F[i+1][a[i+1]]<?=F[i][j];
				// change
				else{
					Rep(t,256)if(abs(j-t)<=M)
						F[i+1][t]<?=F[i][j]+abs(a[i+1]-t);	
				}
			}
		}
		int best=inf;
		Rep(j,256)best<?=F[n][j];
		cout << "Case #" << t+1 << ": ";
		cout<<best;
		cout<<endl;
	}
	return 0;	
}
