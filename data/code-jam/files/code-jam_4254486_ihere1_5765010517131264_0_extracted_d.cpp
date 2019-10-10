#include<iostream>
#include<sstream>
#include<cstdio>
#include<cstring>
#include<string>
#include<cstdlib>
#include<cmath>
#include<cctype>
#include<ctime>
#include<algorithm>
#include<iomanip>
#include<vector>
#include<queue>
#include<map>
#include<set>
#include<cassert>
#include<bitset>

using namespace std;
const int  N = 1000;
long double q1[N], v1[N];
long double q2[N], v2[N];
int f1 , f2;
long double p[N];
int main() {
	int TT;
	scanf("%d", &TT);
	for(int cc = 1; cc <= TT; ++cc){
		long double v;
		int n;
		cin >> v >> n;
		long double t1 = 0, t2 = 0, t3 = 0, t4 = 0;
		for(int i=0; i<n; ++i){
			cin >> p[i];
		}
		int f1 = 0, f2 = 0; 
		for(int i=0; i<n; ++i){
			int x;
			cin >> x;
			if(p[i] < 0){
				q1[f1] = -p[i];
				v1[f1++] = x;
			}else{
				q2[f2] = p[i];
				v2[f2++] = x;
			}
		}
		for(int i=0; i<f1; ++i){
			t1 = max(t1, q1[i]/(v - v1[i]));
		}
		for(int i=0; i<f2; ++i){
			t2 = max(t2, (q2[i]+t1 * v + v2[i] * t1) / (v - v2[i]));
		}
		for(int i=0; i<f2; ++i){
			t3 = max(t3, q2[i]/(v-v2[i]));
		}
		for(int i=0; i<f1; ++i){
			t4 = max(t4, (q1[i] + t3 * v + v1[i] * t3) / (v - v1[i]));
		}
		long double t5 = 0, t6 = 0;
		for(int i = 0; i<f1 ; ++i){
			for(int j = 0; j<f2; ++j){
				t5 = max(t5, (q2[j] * (v - v1[i])+ q1[i] * (v + v) / (v - v1[i]) / (v - v2[j])));
				t6 = max(t6, (q2[j] * (v + v)+ q1[i] * (v - v2[i])) / (v - v1[i]) / (v - v2[j]));
			}
		}
		printf("Case #%d: %.10f\n", cc, (double)(max(t1, max(t3, min(t5, t6)))));
		//printf("Case #%d: %.10f\n", cc, (double)(min(t2 + t1, t4 + t3)));
	}
	return 0;
}

