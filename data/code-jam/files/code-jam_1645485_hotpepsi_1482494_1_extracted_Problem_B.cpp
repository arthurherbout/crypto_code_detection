
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

typedef vector<int> IntVec;
typedef pair<int, int> II;
typedef vector<II> IIVec;
typedef pair<int, II> III;
typedef vector<III> IIIVec;

static int solve(int N, IntVec a, IntVec b) {
	int result = 0;
	IIVec A, B;
	IIIVec C;
	int i;
	for (i = 0; i < N; ++i) {
		A.push_back(II(a[i], i));
		B.push_back(II(b[i], i));
	}
	sort(A.begin(), A.end());
	sort(B.begin(), B.end());

	IntVec f(N);
	int s = 0;
	int r = N;
	int l = 0;
	while (r > 0) {
		if (s >= B[l].first) {
			++result;
//			cout << "2: " << B[l].second << endl;
			s += 2 - f[B[l].second];
			f[B[l].second] = 2;
			--r;
			++l;
			continue;
		}
		int req = B[l].first - s;
		while (req > 0) {
			int j = -1;
			int MaxB = -1;
			for (i = 0; i < N; ++i) {
				if (s < A[i].first) {
					break;
				}
				if (!f[A[i].second]) {
					if (b[A[i].second] > MaxB) {
						j = A[i].second;
						MaxB = b[j];
					}
				}
			}
			if (MaxB < 0) {
				return -1;
			}
			f[j] = 1;
			++result;
			++s;
			--req;
		}

		
		
#if 0		
		/*
		for (i = 0; i < N; ++i) {
			if (s < A[i].first) {
				break;
			}
*/
		for (i = N - 1; i >= 0; --i) {
			if (s < A[i].first) {
				continue;
			}

			if (!f[A[i].second]) {
//				cout << "1: " << A[l].second << endl;
				f[A[i].second] = 1;
				++result;
				++s;
				--req;
				if (req <= 0) {
					break;
				}
			}
		}
		if (req > 0) {
			result = -1;
			break;
		}
#endif
	}

	return result;
}

int main(int argc, char *argv[])
{
	cout.precision(16);
	string s;
	getline(cin, s);
	int T = atoi(s.c_str());
	int t;
	for (t = 0; t < T; ++t) {
		IntVec a, b;
		getline(cin, s);
		int N = atoi(s.c_str());
		int i;
		for (i = 0; i < N; ++i) {
			getline(cin, s);
			stringstream ss(s);
			int p, q;
			ss >> p >> q;
			a.push_back(p);
			b.push_back(q);
		}
		int result = solve(N, a, b);
		if (result < 0) {
			cout << "Case #" << (t+1) << ": Too Bad" << endl;
		} else {
			cout << "Case #" << (t+1) << ": " << result << endl;
		}
	}

	return 0;
}

