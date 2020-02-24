#include <cstdio>

using namespace std;

int main(){
	freopen("A-large.in", "r", stdin);
	freopen("out.txt", "w", stdout);
	
	int T;
	scanf("%d", &T);
	
	for (int tc = 1; tc <= T; ++tc){
		printf("Case #%d:\n", tc);
		int n;
		scanf("%d\n", &n);
		int table[n][n];
		char c;
		for (int i = 0; i < n; ++i){
			for (int j = 0; j < n; ++j){
				scanf("%c", &c);
				if (c == '0') table[i][j] = -1;
				if (c == '.') table[i][j] = 0;
				if (c == '1') table[i][j] = 1;
			}
			scanf("\n");
		}
		
		double wp[n], owp[n], oowp[n];
		for (int i = 0; i < n; ++i){
			wp[i] = 0;
			int opp = 0;
			for (int j = 0; j < n; ++j){
				if (table[i][j] == 1) wp[i]++;
				if (table[i][j] != 0) opp++;
			}
			if (opp != 0) wp[i] = wp[i]/opp;
		}
		for (int i = 0; i < n; ++i){
			owp[i] = 0;
			double wp2[n];
			int o = 0;
			for (int k = 0; k < n; ++k){
				wp2[k] = 0;
				if (k == i) continue;
				if (table[i][k] == 0) continue;
				++o;
				int opp = 0;
				for (int j = 0; j < n; ++j){
					if (j == i) continue;
					
					if (table[k][j] == 1) wp2[k]++;
					if (table[k][j] != 0) opp++;
				}
				if (opp != 0) wp2[k] = wp2[k]/opp;
			}
			for (int k = 0; k < n; ++k) owp[i] += wp2[k];
			owp[i] = owp[i] / (o);
		}
		for (int i = 0; i < n; ++i){
			oowp[i] = 0;
			int opp = 0;
			for (int j = 0; j < n; ++j) {
				if (table[i][j] != 0) {
					oowp[i] += owp[j];
					++opp;
				}
			}
			oowp[i] = oowp[i]/opp;
		}
		for (int i = 0; i < n; ++i){
			printf("%.7f\n", 0.25 * wp[i] + 0.50 * owp[i] + 0.25 * oowp[i]);
			//printf("%f\n", oowp[i]);
		}
	}
	
	return 0;
}
