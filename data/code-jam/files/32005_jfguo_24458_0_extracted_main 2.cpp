#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <queue>
#include <map>
#include <cmath>
#include <sstream>
using namespace std;

typedef long long LL;
typedef unsigned int UINT32;

int big[128][128];
int small[128][128];

int N;
int M;

int bs(int a)
{
	int ans = 0;
	while (a) {
		a &= a-1;
		ans++;
	}
	return ans;
}

string calc()
{
	int i, j;
	int sk1, sk2;
	int bk1, bk2;
	for (i=0; i<(1<<N); ++i) {
		if (bs(i) != M) continue;

		vector<int> seq;
		for (j=0; j<N; ++j) if (i&(1<<j)) {
			seq.push_back(j);
		}

		do {
			bool ok = true;
			for (sk1=0; sk1<M; ++sk1) for (sk2=sk1+1; sk2<M; ++sk2) {
				bk1 = seq[sk1];
				bk2 = seq[sk2];

				if (small[sk1][sk2] && !big[bk1][bk2] || !small[sk1][sk2] && big[bk1][bk2]) {
					ok = false;
					break;
				}
				if (!ok) break;
			}
			if (ok) return "YES";
		} while (next_permutation(seq.begin(), seq.end()));
	}

	return "NO";
}

int main(void)
{
	int cases;
	cin >> cases;

	int i, j;
	for (int c=1; c<=cases; ++c) {
		cin >> N;
		memset(big, 0 ,sizeof(big));
		memset(small, 0 ,sizeof(small));

		int x, y;
		for (i=0; i<N-1; ++i) {
			cin >> x >> y;
			big[x-1][y-1] = big[y-1][x-1] = 1;
		}

		cin >> M;
		for (i=0; i<M-1; ++i) {
			cin >> x >> y;
			small[x-1][y-1] = small[y-1][x-1] = 1;
		}

		/*
		for (i=0; i<N; ++i) {
			for (j=0; j<N; ++j) cout << big[i][j];
			cout << endl;
		}
		cout << endl;
		for (i=0; i<M; ++i) {
			for (j=0; j<M; ++j) cout << small[i][j];
			cout << endl;
		}
		*/

		//cout << "         " << N << ' ' << M << endl;
		cout << "Case #" << c << ": " << calc() << endl;
	}

	return 0;
}
