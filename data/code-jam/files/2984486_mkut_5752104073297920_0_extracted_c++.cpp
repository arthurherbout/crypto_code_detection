#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

vector<int> good() {
	int N = 1000;
	vector<int> vi(N);
	for (int i = 0; i < N; i++) {
		vi[i] = i;
	}
	for (int i = 0; i < N; i++) {
		int j = rand() % (N - i) + i;
		swap(vi[i], vi[j]);
	}
	return vi;
}
vector<int> bad() {
	int N = 1000;
	vector<int> vi(N);
	for (int i = 0; i < N; i++) {
		vi[i] = i;
	}
	for (int i = 0; i < N; i++) {
		int j = rand() % N;
		swap(vi[i], vi[j]);
	}
	return vi;
}

bool check(vector<int>& vi) {
	int N = 1000;
	int border = N*(N-1)/2.0*0.51;
	int cnt = 0;
	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			cnt += vi[i] < vi[j] ? 1 : 0;
		}
	}

	return cnt <= border;
}

int main() {
	/*
	srand(1000);
	int g = 0, b = 0;
	for (int i = 0; i < 1000; i++) {
		vector<int> vi = good();
		if (check(vi)) {
			g++;
		} else {
			b++;
		}
	}
	cout << g << "," << b << endl;

	g = 0, b = 0;
	for (int i = 0; i < 1000; i++) {
		vector<int> vi = bad();
		if (check(vi)) {
			g++;
		} else {
			b++;
		}
	}
	cout << g << "," << b << endl;
	*/
	int T; cin >> T;
	for (int No = 1; No <= T; No++) {
		int N; cin >> N;
		vector<int> vi(N);
		for (int i = 0; i < N; i++) {
			cin >> vi[i];
		}
		cout << "Case #" << No << ": " << (check(vi) ? "GOOD" : "BAD") << endl;
	}
}
