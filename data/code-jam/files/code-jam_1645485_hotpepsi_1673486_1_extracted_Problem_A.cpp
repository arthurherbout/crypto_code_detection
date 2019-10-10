
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

typedef vector<double> DblVec;
typedef vector<int> IntVec;

//static double P[100004];


static double solve(int A, int B, DblVec pv) {
//	memset(P, 0, sizeof(P));
	DblVec P(A+1);
	P[0] = 1.0;
	double p = 1.0;
	int i, j, k;
	for (i = 0; i < A; ++i) {
		p *= pv[i];
		P[i+1] = p;
	}
//	DblVec X(A+1);
	double result = B + 2;
	for (i = 0; i <= A; ++i) {
		int ok = A - i;
		int r = B - A;
		//bs
		double sta = i*2 + r + 1;
		double stb = sta + B + 1;
		p = ((P[ok] * sta) + ((1.0 - P[ok]) * stb));
		result = min(result, p);
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
		int A, B;		// A: typed, B: total length
		DblVec pv;
		getline(cin, s);
		{
			stringstream ss(s);
			ss >> A >> B;
		}
		getline(cin, s);
		{
			stringstream ss(s);
			int i;
			for (i = 0; i < A; ++i) {
				double d;
				ss >> d;
				pv.push_back(d);
			}
		}
		double result = solve(A, B, pv);
		cout << "Case #" << (t+1) << ": " << result << endl;
	}

	return 0;
}

