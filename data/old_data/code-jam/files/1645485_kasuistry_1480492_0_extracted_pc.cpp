
#include <cstdio>

double abs(double a){
	return (a > 0) ? a : -a;
}

int minima(int a, int b){
	return (a > b) ? b : a;
}

int maxima(int a, int b){
	return (a > b) ? a : b;
}

int main(){
	
	int t; scanf("%d", &t);
	int s[50];
	int p[50];

	for(int testcase=1; testcase<=t; ++testcase){
		char dir[100];
		int n; scanf("%d", &n);
		for(int i=0; i<n; ++i){
			scanf("%s %d %d", dir, &s[i], &p[i]);
		}
		double ans = -1;
		for(int i=0; i<n; ++i){
			for(int j=i+1; j<n; ++j){
				for(int k=j+1; k<n; ++k){
					double t;
					int pivot;
					if(s[i]==s[j]&&s[j]==s[k]){
						continue;
					}else if(s[i]==s[j]){
						if(abs(p[i]-p[j]) > 5){
							continue;
						}else{
							if(s[k] > s[j]){
								pivot = maxima(p[j], p[i]);
							}else{
								pivot = minima(p[j], p[i])+5;
							}
							t = ((double)(pivot-p[k]))/(s[k]-s[i]);
						}
					}else if(s[j]==s[k]){
						if(abs(p[j]-p[k]) > 5){
							continue;
						}else{
							if(s[i] > s[k]){
								pivot = maxima(p[j], p[k]);
							}else{
								pivot = minima(p[j], p[k])+5;
							}
							t = ((double)(p[i]-pivot))/(s[k]-s[i]);
						}
					}else if(s[i]==s[k]){
						if(abs(p[i]-p[k]) > 5){
							continue;
						}else{
							if(s[j] > s[i]){
								pivot = maxima(p[i], p[k]);
							}else{
								pivot = minima(p[i], p[k])+5;
							}
							t = ((double)(pivot-p[j]))/(s[j]-s[i]);
						}
					}else{
						double ta, tb, tc, td, te, tf;
						ta = ((double)(p[i]+5-p[j]))/(s[j]-s[i]);
						tb = ((double)(p[i]-5-p[j]))/(s[j]-s[i]);
						tc = ((double)(p[j]+5-p[k]))/(s[k]-s[j]);
						td = ((double)(p[j]-5-p[k]))/(s[k]-s[j]);
						te = ((double)(p[i]+5-p[k]))/(s[k]-s[i]);
						tf = ((double)(p[i]-5-p[k]))/(s[k]-s[i]);
						double swap;
						if(ta > tb){
							swap = ta; ta = tb; tb = swap;
						}
						if(tc > td){
							swap = tc; tc = td; td = swap; 
						}
						if(te > tf){
							swap = te; te = tf; tf = swap; 
						}
						if(ta < 0 && tb >=0){
							ta = 0;
						}
						if(tc < 0 && td >= 0){
							tc = 0;
						}
						if(te < 0 && tf >= 0){
							te = 0;
						}
						if(tc >= ta && tc <= tb && tc >= te && tc <= tf){
							t = tc;
						}else if(ta >= tc && ta <= td && ta >= te && ta <=tf){
							t = ta;
						}else{
							t = tb;
						}
					}
					//printf("%lf\n", t);
					if(t >= 0){
						if(ans < -0.5){
							ans = t;
						}else{
							if(ans > t){
								ans = t;
							}
						}
					}
				}
			}
		}
		if(ans < -0.5){
			printf("Case #%d: Possible\n", testcase);
		}else{
			printf("Case #%d: %.6lf\n", testcase, ans);
		}
	}
	return 0;
}
