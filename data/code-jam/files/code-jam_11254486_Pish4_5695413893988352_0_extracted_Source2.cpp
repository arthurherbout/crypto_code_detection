#include <vector>
#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <string>
#include <queue>

using namespace std;

typedef long long ll;
fstream in("B-small-attempt2.in");
fstream out("out.txt");

string cycle(string a, char old, char nw){
	string r = a;
	for (int i = 0; i < (int)r.size(); i++){
		if (r[i] == old) r[i] = nw;
	}
	return r;
}

ll toll(string a){
	ll r = 0;
	for (int i = 0; i < (int)a.size(); i++) {
		r = r * 10 + a[i] - '0';
	}
	return r;
}

string A, B;
ll ee = 1e18;

void maximize(string a, string b){
	ll x = toll(a);
	ll y = toll(b);
	if (abs(x - y) < ee ||
		abs(x - y) == ee && a < A ||
		abs(x - y) == ee && a == A && b < B) {
		A = a;
		B = b;
		ee = abs(x - y);
	}
}

char eq(char a, char b){
	return a == '?' || b == '?' || a == b;
}

void foo(){
	ee = 1e18;
	string a, b, x, y;
	in >> a >> b;
	int n = a.size();

	int max_diff = 0;
	for (; max_diff < n && eq(a[max_diff], b[max_diff]); max_diff++);

	//cout << max_diff << endl;
	for (int i = 0; i < max_diff; i++) {
		int la, ra, lb, rb;
		if (a[i] == '?') la = '0', ra = '9';
		else la = ra = a[i];

		if (b[i] == '?') lb = '0', rb = '9';
		else lb = rb = b[i];

		x = a;
		y = b;
		for (int j = 0; j < i; j++) {
			int c;
			if (a[j] != '?' && b[j] != '?')
				c = a[j];
			else if (a[j] == '?' && b[j] == '?')
				c = '0';
			else
				c = a[j] + b[j] - '?';
			x[j] = y[j] = c;
		}
		//cout << x << " " << y << endl;
		for (x[i] = la; x[i] <= ra; x[i]++){
			for (y[i] = lb; y[i] <= rb; y[i]++){
				maximize(cycle(x, '?', '0'), cycle(y, '?', '9'));
				maximize(cycle(x, '?', '9'), cycle(y, '?', '0'));
			}
		}
	}
	maximize(cycle(a, '?', '0'), cycle(b, '?', '9'));
	maximize(cycle(a, '?', '9'), cycle(b, '?', '0'));

	out << A << " " << B << endl;
}

int main() {

	int num;
	in >> num;
	for (int test = 1; test <= num; test++){
		out << "Case #" + to_string(test) + ": ";
		foo();
	}
	return 0;
}