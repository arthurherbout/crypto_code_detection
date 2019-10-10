// Google Code Jam 2014 R2
// Problem B. 

#include <iostream>
#include <sstream>
#include <algorithm>
#include <set>
#include <vector>

using namespace std;

typedef vector<int> IntVec;

int solve(IntVec &v)
{
	int ans = 1 << 30;
	int N = v.size();
#if 0
	int left[1024];
	int right[1024];
	for (int i = 0; i < N; ++i) {
		int c = 0;
		for (int j = 0; j < i; ++j) {
			c += v[j] > v[i];
		}
		left[i] = c;
		c = 0;
		for (int j = i+1; j < N; ++j) {
			c += v[j] > v[i];
		}
		right[i] = c;
	}
	for (int mpos = 0; mpos <= N; ++mpos) {
		int c = 0;
		for (int j = 0; j < mpos; ++j) {
			c += left[j];
		}
		for (int j = mpos; j < N; ++j) {
			c += right[j];
		}
		ans = min(ans, c);
	}
#endif

#if 0
	for (int mpos = 0; mpos <= N; ++mpos) {
		IntVec a(v.begin(), v.begin() + mpos);
		IntVec b(v.begin() + mpos, v.end());
		int c = 0;
		if (a.size() > 0) {
			bool f = true;
			while (f) {
				f = false;
				for (int i = 1; i < (int)a.size(); ++i) {
					if (a[i] < a[i - 1]) {
						swap(a[i - 1], a[i]);
						f = true;
						++c;
						break;
					}
				}
			}
		}
		if (b.size() > 0) {
			bool f = true;
			while (f) {
				f = false;
				for (int i = 1; i < (int)b.size(); ++i) {
					if (b[i] > b[i - 1]) {
						swap(b[i - 1], b[i]);
						f = true;
						++c;
						break;
					}
				}
			}
		}
		ans = min(ans, c);
	}
#endif

	int left[1024];
	int right[1024];
	for (int i = 0; i < N; ++i) {
		int c = 0;
		for (int j = 0; j < i; ++j) {
			c += v[j] > v[i];
		}
		left[i] = c;
		c = 0;
		for (int j = i + 1; j < N; ++j) {
			c += v[j] > v[i];
		}
		right[i] = c;
	}

	int M = 1 << N;
	for (int m = 0; m < M; ++m) {
		int b = 1;
		int c = 0;
		for (int i = 0; i < N; ++i, b <<= 1) {
			if (m & b) {
				c += right[i];
			} else {
				c += left[i];
			}
		}
		ans = min(ans, c);
	}

	return ans;
}

int main(int argc, char *argv[])
{
	string s;
	getline(cin, s);
	int T = atoi(s.c_str());
	if (T <= 0) return 0;

	for (int t = 1; t <= T; ++t) {
		int N = 0;

		getline(cin, s);
		{
			stringstream ss(s);
			ss >> N;
		}

		IntVec v(N);
		getline(cin, s);
		{
			stringstream ss(s);
			for (int i = 0; i < N; ++i) {
				ss >> v[i];
			}
		}

		int ans = solve(v);
		printf("Case #%d: %d\n", t, ans);
	}

	return 0;
}

