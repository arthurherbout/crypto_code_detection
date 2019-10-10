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


int val[1111111];
const int N = 1048576;
int w[2 * N + 1];

void Add(int pos)
{
	w[N + pos] = 0;
	int i = N + pos;
	int j = i / 2;
	while (j > 0) {
		w[j] = w[j << 1] + w[(j << 1) + 1];
		j >>= 1;
	}
}

int Count(int pos)
{
	int l, r;
	l = N;
	r = N + pos;
	int res = 0;
	while (l <= r && l > 0) {
		if (l % 2 == 1) {
			res += w[l];
			l++;
		}
		if (r % 2 == 0) {
			res += w[r];
			r--;
		}
		l >>= 1; r >>= 1;
	}
	return res;
}

int Find(int k, int ver)
{
	int pos = 1;
	if (ver >= N) return ver - N;
	if (w[ver<<1] >= k) return Find(k, ver<<1);
	else return Find(k - w[ver<<1], (ver<<1) + 1);
}

void Solve()
{
	int i, j, k;
	int pos = 1;
	int rpos;
	int npos;
	memset(val, 0, sizeof(val));
	for (i = 1; i <= K; i++) {
		w[i + N] = 1;
	}
	for (i = N - 1; i > 0; i--) w[i] = w[i * 2] + w[i * 2 + 1];
	for (i = 1; i <= K; i++) {
		npos = Find(((pos + i - 2) % w[1] + 1), 1);
		val[npos] = i;
		Add(npos);
		pos = Count(npos) + 1;
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