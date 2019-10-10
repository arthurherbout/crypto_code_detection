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
		double p[100][3];
		double mini = 2147483647.0;
		cin >> n;
		for(int i=0;i<n;i++) cin >> p[i][0] >> p[i][1] >> p[i][2];
		if(n<=2){
			if(n==1)
				printf("Case #%d: %lf\n",cas,p[0][2]);
			else 
				printf("Case #%d: %lf\n",cas,max(p[0][2],p[1][2]));
		}else{
			for(int i=0;i<n;i++){
				for(int j=i+1;j<n;j++){
					double dx = p[i][0] - p[j][0];
					double dy = p[i][1] - p[j][1];
					double dis = sqrt(dx*dx+dy*dy);
					mini = min(mini,dis + (p[i][2]+p[j][2]));
//					printf("%lf\n",mini);
				}
			}
			mini /= 2;
			for(int i=0;i<n;i++){
				mini = max(mini,p[i][2]);
			}
			printf("Case #%d: %lf\n",cas,mini);
		}
	}
	return 0;
}
