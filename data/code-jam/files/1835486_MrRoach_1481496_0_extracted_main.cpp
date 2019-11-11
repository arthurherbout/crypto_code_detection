#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <deque>

using namespace std;
const string maplist = "48cd3f9h1jklmn0pqr57uvwxyz";

int T, k;
string s;
bool m[128][128];
int len;

int main(){
	cin >> T;
	for (int t = 0; t < T; ++t) {
		memset(m, false, sizeof(m));
		len = 0;
		cin >> k;
		cin >> s;
		int n = s.length();
		for (int i = 0; i < n-1; ++i){
			m[s[i]][s[i+1]] = true;
			m[s[i]][maplist[s[i+1]-'a']] = true;
			m[maplist[s[i]-'a']][s[i+1]] = true;
			m[maplist[s[i]-'a']][maplist[s[i+1]-'a']] = true;
		}
		int total = 0;
		for (int i = 0; i < 128; ++i) {
			int id = 0, od = 0;
			for (int j = 0; j < 128; ++j) {
				if (m[i][j]){
					++ od;
					++len;
				}
				if (m[j][i]){
					++ id;
				}
			}
			if (od > id) {
				total += od-id;
			}
		}
		if (total <= 1) len += 1;
		else len += total;
		cout << "Case #" << t + 1 << ": " << len << endl;
	}
	return 0;
}