#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <map>

const long double eps = 1e-9;

using namespace std;

int n, K;
int d[111];
void Load()
{
	cin >> K >> n;
	int i;
	for (i = 1; i <= n; i++) scanf("%d", &d[i]);
}


int val[5111];

void Solve()
{
	int i, j, k;
	int pos = 1;
	memset(val, 0, sizeof(val));
	for (i = 1; i <= K; i++) {
		j = 1;
		while (j < i) {
			j++;
			pos++;
			if (pos > K) pos = 1;
			while (val[pos] != 0) {
				pos++;
				if (pos > K) pos = 1;
			}
		}
		val[pos] = i;
		pos++;
		if (i == K) break;
		if (pos > K) pos = 1;

		while (val[pos] != 0) {
			pos++;
			if (pos > K) pos = 1;
		}
	}
	for (i = 1; i <= n; i++) printf(" %d", val[d[i]]);
	printf("\n");
}

void Save()
{
}

int main(){
	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);
	int t, tt;
	cin >> tt;
	for (t = 1; t <= tt; t++){ 
		cerr << t << "\n";
		cout << "Case #" << t << ":"; 
		Load();
		Solve();
		Save();
	}
	return 0;
}