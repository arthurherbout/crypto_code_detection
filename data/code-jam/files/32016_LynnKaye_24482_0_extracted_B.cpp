#include <algorithm>
#include <vector>
#include <cstdio>
#include <queue>
using namespace std;

const int MaxN = 2000 + 100;
const int MaxM = 2000 + 100;

int caseNum, totCase;

int n, m;

queue <int> cos[MaxM];
int cosSize[MaxM];
int malted[MaxN], id[MaxN];
int flag[MaxN];

void init();
void work();

int main() {
	scanf("%d", &totCase);
	for (caseNum = 1; caseNum <= totCase; ++caseNum) {
		printf("Case #%d:", caseNum);
		init();
		work();
	}
	return 0;
}

void init() {
	scanf("%d", &n);
	scanf("%d", &m);
	for (int i = 0; i < m; ++i) {
		while (!cos[i].empty())
			cos[i].pop();
	}
	for (int i = 0; i < m; ++i) {
		scanf("%d", &cosSize[i]);
		malted[i] = 0;
		int newSize = cosSize[i];
		for (int j = 0; j < cosSize[i]; ++j) {
			int a, b;
			scanf("%d %d", &a, &b);
			--a;
			if (b == 0)
				cos[i].push(a);
			else {
				malted[i] = 1;
				id[i] = a;
				--newSize;
			}
		}
		cosSize[i] = newSize;
	}
}

void work() {
	fill(flag, flag + n, 0);
	bool finish = false;
	while (!finish) {
		finish = true;
		for (int i = 0; i < m; ++i) {
			bool tmp = true;
			int newSize = cosSize[i];
			for (int j = 0; j < cosSize[i]; ++j) {
				int cur = cos[i].front();
				cos[i].pop();
				if (flag[cur] == 0) {
					cos[i].push(cur);
					tmp = false;
					break;
				}
				--newSize;
			}
			cosSize[i] = newSize;
			if (tmp) {
				if (malted[i] == 1) {
					if (flag[id[i]] != 1) {
						flag[id[i]] = 1;
						finish = false;
					}
				} else {
					printf(" IMPOSSIBLE\n");
					return;
				}
			}
		}
	}
	for (int i = 0; i < n; ++i)
		printf(" %d", flag[i]);
	printf("\n");
}
