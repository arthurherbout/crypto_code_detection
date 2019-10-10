#include <iostream>
#include <string>
#include <string.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <bitset>

using namespace std;

int n,b;
int a[201];
int used[30][30];
int res=0;
int vv=0;

void rec(int sum, int s){
	if (sum==n){
		res++;
		//for (int i=1; i<=vv; i++)
		//	printf("%d ",a[i]);
		//printf("\n");
		return;
	}
	for (int i=s; sum+i<=n; i++){
		int dig[20];
		int ver=0;
		int m=i;
		while (m>0){
			ver++;
			dig[ver]=m%b;
			m/=b;
		}

		int k=0;
		for (int j=1; j<=ver; j++){
			if (used[j][dig[j]]){
				k=1;
				break;
			}
		}
		if (k) continue;
		for (int j=1; j<=ver; j++)
			used[j][dig[j]]=1;
		//vv++;
		//a[vv]=i;
		rec(sum+i,i+1);
		//vv--;
		for (int j=1; j<=ver; j++)
			used[j][dig[j]]=0;
	}
}

int main(){
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);

	int t;
	scanf("%d",&t);

	for (int tt=1; tt<=t; tt++){
		printf("Case #%d: ",tt);

		scanf("%d%d",&n,&b);
		res=0;
		memset(used,0,sizeof(used));
		vv=0;
		rec(0,1);
		printf("%d\n",res);
	}

	return 0;
}