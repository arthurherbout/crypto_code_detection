#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

using ull = unsigned long long;

void addnc(string& s, int n, char c) {
	for (int i = 0;i < n;++i)
		s += c;
}

string SolveA(string& s) {
	//sort(s.begin(), s.end());
	int z = count(s.begin(), s.end(), 'Z');
	int w = count(s.begin(), s.end(), 'W');
	int u = count(s.begin(), s.end(), 'U');
	int x = count(s.begin(), s.end(), 'X');
	int o = count(s.begin(), s.end(), 'O');
	int f = count(s.begin(), s.end(), 'F');
	int r = count(s.begin(), s.end(), 'R');
	int ss = count(s.begin(), s.end(), 'S');
	int g = count(s.begin(), s.end(), 'G');
	int i = count(s.begin(), s.end(), 'I');
	int zeros = z;
	int twos = w;
	int fours = u;
	int threes = r - zeros - fours;
	int ones = o - zeros - fours - twos;
	int fives = f - fours;
	int sixes = x;
	int sevens = ss - sixes;
	int eights = g;
	int nine = i - sixes - fives - eights;

	string result;
	addnc(result, zeros, '0');
	addnc(result, ones, '1');
	addnc(result, twos, '2');
	addnc(result, threes, '3');
	addnc(result, fours, '4');
	addnc(result, fives, '5');
	addnc(result, sixes, '6');
	addnc(result, sevens, '7');
	addnc(result, eights, '8');
	addnc(result, nine, '9');
	return result;
}

void main() {

	int T;
	cin >> T;
	for (int i = 0;i < T;++i) {
		string s;
		cin >> s;

		cout << "Case #" << i + 1 << ": " << SolveA(s) << endl;
	}
}