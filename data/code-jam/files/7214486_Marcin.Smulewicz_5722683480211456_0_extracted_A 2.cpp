#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
typedef pair<int,int> PI;
typedef double D;
#define MP make_pair
#define FI first
#define SE second
#define PB push_back
#define R(i,n) for(int i=0;i<n;i++)
#define F(i,a,b) for(int i=a;i<b;i++)
#define FD(i,n) for(int i=n-1;i>=0;i--)
#define make(a) scanf("%d",&a)
template<typename A> void ma(A &a, A b){if(a<b)a=b;}
template<typename A> void mi(A &a, A b){if(a>b)a=b;}
#define MAX 1001
int n;
char t[MAX][MAX];
void nie(){
	puts("IMPOSSIBLE");
}
void test(){
	static int tes=0;tes++;
	printf("Case #%d: ",tes);
	make(n);n*=2;
	R(i,n)scanf(" %s",t[i]);
	int il=0,il2=0;
	R(i,n){
		il+= (t[0][i]=='1');
		il2+= (t[i][0]=='1');
	}
	if(il*2!=n || il2*2!=n){
		nie();
		return;
	}
	R(i,n)R(j,n){
		if(t[i][j]^t[i][0]^t[0][j]^t[0][0]){
			nie();
			return;
		}
	}
	il = 0;il2 = 0;
	for(int i=0;i<n;i+=2){
		if(t[i][0]!=t[0][0])il++;
		if(t[0][i]!=t[0][0])il2++;
	}
	printf("%d\n",min(il+il2,min(n/2+il-il2,n/2-il+il2)));
}
main(){
	int _;make(_);while(_--)test();
}