#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <map>
#include <cmath>
#include <cstdlib>
#include <cstring>

#define ll long long

using namespace std;

int main(){
	int ncas;
	scanf("%d",&ncas);
	for(int cas=1;cas<=ncas;cas++){
		int n;
		int s[64];
		scanf("%d",&n);
		for(int i=0;i<n;i++){
			s[i] = 0;
			for(int j=0;j<n;j++){
				char ch;
				scanf(" %c",&ch);
				if(ch == '1') s[i] = j;
			}
		}
		int nswap = 0;
		bool flag = 1;
		while(flag){
			flag = 0;
			for(int i=n-1;i>=0;i--){
				if(s[i]>i){
					for(int j=i+1;j<n;j++){
						if(s[j] <= i){
						//	printf("j:%d",j);
							for(int k=j-1;k>=i;k--){
								swap(s[k+1],s[k]);
								nswap++;
							}
							break;
						}
					}
					flag = 1;
					break;
				}
			}
//			for(int i=0;i<n;i++) printf("%d:%d\n",i,s[i]);
//			printf("%d\n",nswap);
		}

		printf("Case #%d: %d\n",cas,nswap);
	}
	return 0;
}
