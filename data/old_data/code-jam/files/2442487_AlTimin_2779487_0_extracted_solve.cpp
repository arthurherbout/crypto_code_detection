#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cassert>

using namespace std;

bool check(const vector<int>& x, const vector<int>& a, const vector<int>& b) {
	vector <int> A(x.size(), 1);
	vector <int> B(x.size(), 1);
	for (int i = 0; i < x.size(); i ++) {
		for (int j = 0; j < i; j ++) {
			if (x[j] < x[i]) {
				A[i] = max(A[i], A[j] + 1);
			}
		}
		if (A[i] != a[i]) {
			return false;
		}
	}
	for (int i = (int) x.size() - 1; i >= 0; i --) {
		for (int j = i + 1; j < x.size(); j ++) {
			if (x[i] > x[j]) {
				B[i] = max(B[i], B[j] + 1);
			}
		}
		if (B[i] != b[i]) {
			return false;
		}
	}
	return true;
}

int N;

const int nmax = 2000;
bool used[nmax];

vector <int> x, A, B;

bool go(int n) {
	if (n == N) {
		return check(x, A, B);
	}
	for (int i = 0; i < N; i ++) {
		if (!used[i]) {
			int a = 1;
			for (int j = 0; j < n; j ++) {
				if (x[j] < i) {
					a = max(a, A[j] + 1);
				}
			}
			if (A[n] == a) 
			{
				used[i] = 1;
				x.push_back(i);
				if (go(n + 1)) { return true; };
				x.pop_back();
				used[i] = 0;
			}
		}
	}
	return false;
}

void solve(int tn) {
	int n;
	cin >> n;
	N = n;
	for (int i = 0; i < n; i ++) {
		used[i] = false;
	}
	vector<int> a, b;
	for (int i = 0; i < n; i ++) {
		int t;
		cin >> t;
		a.push_back(t);
	}
	for (int i = 0; i < n; i ++) {
		int t;
		cin >> t;
		b.push_back(t);
	}
	x.clear();
	A = a; 
	B = b;
	assert(go(0));
	cout << "Case #" << tn << ":";
	for (int i = 0; i < n; i ++) {
		cout << " " << x[i] + 1;
	} 
	cout << endl;
}

int main() {
	int T;
	cin >> T;
	for (int i = 0; i < T; i ++) {
		solve(i + 1);
	}
}