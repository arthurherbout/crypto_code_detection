#include <cstdio>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <stack>
#include <cstring>
#include <cstdlib>
#include <cmath>

using namespace std;
typedef long long i64;
#define MOD @
#define ADD(X,Y) ((X) = ((X) + (Y)) % MOD)
#define DBG(...) (fprintf(stderr, "[line %d]: ", __LINE__), fprintf(stderr, __VA_ARGS__))

char in[1010101];
int L;

int dpL[1010101][182], dpR[1010101][182];

int main()
{
	int n_testcase;
	scanf("%d", &n_testcase);
	
	for (int testcase_id = 0; testcase_id++ < n_testcase; ) {
		// input and solve the problem
		scanf("%s", in); L = strlen(in);
		
		for (int i = 0; i <= L; ++i) {
			for (int j = 0; j <= 180; ++j) {
				dpL[i][j] = i;
				dpR[i][j] = i;
			}
		}
		int ret = 11111;
		for (int j = 0; j <= 180; ++j) {
			for (int i = 0; i < L; ++i) {
				int v = in[i] - '0';
				if (v <= j) {
					dpL[i][j] = max(dpL[i][j], i + 1);
					dpR[i + 1][j] = min(dpR[i + 1][j], i);
					
					dpL[dpR[i][j - v]][j] = max(dpL[dpR[i][j - v]][j], dpL[i + 1][j - v]);
					dpR[dpL[i + 1][j - v]][j] = min(dpR[dpL[i + 1][j - v]][j], dpR[i][j - v]);
				}
			}
			
			for (int i = 0; i < L; ++i) dpL[i + 1][j] = max(dpL[i + 1][j], dpL[i][j]);
			for (int i = L; i >= 1; --i) dpR[i - 1][j] = min(dpR[i - 1][j], dpR[i][j]);
			
			if (dpL[0][j] == L) {
				ret = j;
				break;
			}
		}

		printf("Case #%d: %d\n", testcase_id, ret);
		// print answer here
	}
	return 0;
}

