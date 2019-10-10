#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

using ull = unsigned long long;

int D(string A, string B) {
	stringstream ssA(A);
	stringstream ssB(B);
	int a, b;
	ssA >> a;
	ssB >> b;
	return (b - a);
}


string SolveB(string& A, string& B) {
	int N = A.length();
	vector<string> biggestA(N+1);
	vector<string> biggestB(N+1);
	vector<string> smallestA(N+1);
	vector<string> smallestB(N+1);
	vector<string> closestA(N+1);
	vector<string> closestB(N+1);
	for (int i = 1;i <= N;++i) {
		string suba = A.substr(N - i);
		replace(suba.begin(), suba.end(), '?', '0');
		smallestA[i] = suba;
		string subb = B.substr(N - i);
		replace(subb.begin(), subb.end(), '?', '0');
		smallestB[i] = subb;
		suba = A.substr(N - i);
		replace(suba.begin(), suba.end(), '?', '9');
		biggestA[i] = suba;
		subb = B.substr(N - i);
		replace(subb.begin(), subb.end(), '?', '9');
		biggestB[i] = subb;

		char a1, a2, a3, b1, b2, b3;
		if (A[N - i] == '?' && B[N - i] == '?') {
			a1 = '0';
			b1 = '0';
			a2 = '0';
			b2 = '1';
			a3 = '1';
			b3 = '0';
		}
		else if (B[N - i] == '?') {
			a1 = A[N - i];
			b1 = a1;
			a2 = A[N - i];
			b2 = a2 + 1;
			a3 = A[N - i];
			b3 = a3 - 1;
		}
		else if (A[N - i] == '?') {
			b1 = B[N - i];
			a1 = b1;
			b2 = B[N - i];
			a2 = b2 - 1;
			b3 = B[N - i];
			a3 = b3 + 1;
		}
		else {
			a1 = A[N - i];
			a2 = a1;
			a3 = a1;
			b1 = B[N - i];
			b2 = b1;
			b3 = b1;
		}

		string sa1, sa2, sa3, sb1, sb2, sb3;
		sa1 = a1 + closestA[i - 1];
		sb1 = b1 + closestB[i - 1];
		sa2 = a2 + biggestA[i - 1];
		sb2 = b2 + smallestB[i - 1];
		sa3 = a3 + smallestA[i - 1];
		sb3 = b3 + biggestB[i - 1];

		int d1 = D(sa1, sb1);
		int d2 = D(sa2, sb2);
		int d3 = D(sa3, sb3);
		closestA[i] = sa1;
		closestB[i] = sb1;
		if (a2 >='0' && a2 <='9' &&b2>='0' && b2 <='9' && (
			abs(d2) < abs(d1) || 
			(abs(d2)==abs(d1) && sa2 < sa1) ||
			(abs(d2) == abs(d1) && sa2==sa1 && sb2<sb1))) {
			closestA[i] = sa2;
			closestB[i] = sb2;
			d1 = d2;
			sa1 = sa2;
			sb1 = sb2;
		}
		if (a3 >= '0' && a3 <= '9' &&b3 >= '0' && b3 <= '9' && (
			abs(d3) < abs(d1) ||
			(abs(d3) == abs(d1) && sa3 < sa1) ||
			(abs(d3) == abs(d1) && sa3 == sa1 && sb3<sb1))) {
			closestA[i] = sa3;
			closestB[i] = sb3;
		}
	}
	return closestA[N] + ' ' + closestB[N];

}

void main() {

	int T;
	cin >> T;
	for (int i = 0;i < T;++i) {
		string A,B;
		cin >> A >> B;

		cout << "Case #" << i + 1 << ": " << SolveB(A,B) << endl;
	}
}