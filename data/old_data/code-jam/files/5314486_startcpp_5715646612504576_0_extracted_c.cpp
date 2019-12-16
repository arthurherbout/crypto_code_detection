//2ÉpÉ^Å[ÉìééÇ∑
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int T;
int h, w;
string s[50];

bool checkW(vector<string> s) {
	int x;
	
	for (int y = 0; y < h; y++) {
		vector<int> pos;
		
		pos.push_back(-1);
		for (x = 0; x < w; x++) {
			if (s[y][x] == '#') {
				pos.push_back(x);
			}
		}
		pos.push_back(w);
		
		for (int i = 0; i < pos.size() - 1; i++) {
			int cnt = 0;
			for (x = pos[i] + 1; x < pos[i + 1]; x++) {
				if (s[y][x] == '-') {
					cnt++;
				}
			}
			if (pos[i + 1] - pos[i] >= 2 && cnt != 1) {
				return false;
			}
		}
	}
	return true;
}

bool checkH(vector<string> s) {
	int y;
	
	for (int x = 0; x < w; x++) {
		vector<int> pos;
		
		pos.push_back(-1);
		for (y = 0; y < h; y++) {
			if (s[y][x] == '#') {
				pos.push_back(y);
			}
		}
		pos.push_back(h);
		
		for (int i = 0; i < pos.size() - 1; i++) {
			int cnt = 0;
			for (y = pos[i] + 1; y < pos[i + 1]; y++) {
				if (s[y][x] == '|') {
					cnt++;
				}
			}
			if (pos[i + 1] - pos[i] >= 2 && cnt != 1) {
				return false;
			}
		}
	}
	return true;
}

int main() {
	cin >> T;
	for (int id = 1; id <= T; id++) {
		cin >> h >> w;
		for (int i = 0; i < h; i++) cin >> s[i];
		
		vector<string> t;
		t.resize(h);
		for (int i = 0; i < h; i++) {
			t[i] = s[i];
			for (int j = 0; j < w; j++) {
				if (t[i][j] == '-' || t[i][j] == '|') {
					t[i][j] = '-';
				}
			}
		}
		if (checkW(t)) {
			cout << "Case #" << id << ": " << "POSSIBLE" << endl;
			for (int i = 0; i < h; i++) {
				cout << t[i] << endl;
			}
			continue;
		}
		
		vector<string> u;
		u.resize(h);
		for (int i = 0; i < h; i++) {
			u[i] = s[i];
			for (int j = 0; j < w; j++) {
				if (u[i][j] == '-' || u[i][j] == '|') {
					u[i][j] = '|';
				}
			}
		}
		if (checkH(u)) {
			cout << "Case #" << id << ": " << "POSSIBLE" << endl;
			for (int i = 0; i < h; i++) {
				cout << u[i] << endl;
			}
			continue;
		}
		
		cout << "Case #" << id << ": " << "IMPOSSIBLE" << endl;
	}
	return 0;
}