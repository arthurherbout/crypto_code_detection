#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <math.h>
#include <iomanip>
#include <numeric>
#include <stdlib.h>
#include <algorithm>
using namespace std;

int N, C;
const int maxn = 5000;
double res[41][maxn];

long long CC(int n, int k) {
	long long res = 1;
	for (int i = 0; i < k; i++)
		res *= n - i, res /= i + 1;
	return res;
}

long double prob(int have, int extra) {
	long double res = CC(have, C - extra);
	res /= CC(N, C);
	res *= CC(N - have, extra);
	return res;
}

int main() {
	ifstream fin("C-small-attempt0.in"); ofstream fout("Cout.txt");
	int t; fin >> t;
	for (int tt = 0; tt < t; tt++) {
		fin >> N >> C;
		memset(res, 0, sizeof(res));
		
		res[C][1] = 1;
		for (int i = 1; i < maxn; i++) {
			for (int have = C; have < N; have++)
				for (int get = 0; get <= C; get++)
					res[have][i] += res[have - get][i - 1] * prob(have - get, get);

			for (int get = 1; get <= C; get++)
					res[N][i] += res[N - get][i - 1] * prob(N - get, get);
			if (res[N][i] * i < 1e-9)
				break;
		}

		double ans = 0;
		for (int i = 0; i < maxn; i++)
			ans += i * res[N][i];
		fout << "Case #" << tt + 1 << ": " << ans << endl;
	}
	return 0;
}
