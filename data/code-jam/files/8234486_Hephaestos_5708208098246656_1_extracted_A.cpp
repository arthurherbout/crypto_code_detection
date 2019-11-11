#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <limits>
#include <deque>
#include <cstdint>

using namespace std;

typedef int64_t ll;

bool Check(const vector<vector<char>>& b, char direction, int j, int k) {
	int step_x=0;
	int step_y=0;
	int C = b[0].size();
	int R = b.size();
	switch (direction) {
	case '^':
		step_x = 0;
		step_y = -1;
		break;
	case '<':
		step_x = -1;
		step_y = 0;
		break;
	case '>':
		step_x = 1;
		step_y = 0;
		break;
	case 'v':
		step_x = 0;
		step_y = 1;
		break;
	}
	int n_step = 1;
	while (j + n_step*step_y >= 0 && j + n_step*step_y < R &&k + n_step*step_x>=0 &&k + n_step*step_x < C) {
		if (b[j + n_step*step_y][k + n_step*step_x] != '.')
			return true;
		++n_step;
	}
	return false;
}

void main() {
	int T;
	cin >> T;
	
	for (int i = 0; i < T; ++i) {
		int R, C;
		cin >> R >> C;
		vector<vector<char>> b;
		for (int j = 0;j < R;++j) {
			b.push_back(vector<char>(C));
			for (int k = 0;k < C;++k) {
				char c;
				cin >> c;
				b[j][k] = c;
			}
		}
		int answer = 0; 
		bool possible = true;
		for (int j = 0;j < R;++j) {
			if (!possible)
				break;
			for (int k = 0;k < C;++k) {
				if (!possible)
					break;
				if (b[j][k] != '.'&&!Check(b, b[j][k], j, k)) {
					possible = false;
					for (auto d : { '<','>','v','^' }) {
						if (Check(b, d, j, k)) {
							++answer;
							possible = true;
							break;
						}
					}
				}
			}
		}
		if (possible) {
			cout << "Case #" << i + 1 << ": " << answer << endl;
		}
		else {
			cout << "Case #" << i + 1 << ": " << "IMPOSSIBLE"<< endl;
		}
	}
}