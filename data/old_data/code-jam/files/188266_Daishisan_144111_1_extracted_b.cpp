#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct Inter {
	int S, W, T;
	int time;
};

struct Pair {
	int x, y, corner, time;
};

int time_needed_for_intersection(const vector<vector<vector<Inter> > >& table, int x, int y, int time, bool NS) {
	int S = table[x][y][0].S;
	int W = table[x][y][0].W;
	int T = table[x][y][0].T;
	if (NS) {
		if (time < T) {
			int mod = (T - time) % (S + W);
			mod = (mod == 0) ? S + W : mod;
			assert(mod <= S + W);
			if (mod >= W + 1)
				return 1;
			return mod + 1;
		} else {
			int mod = (time - T) % (S + W);
			if (mod < S)
				return 1;
			return (S + W) - mod + 1;
		}
	} else {
		if (time < T) {
			int mod = (T - time) % (S + W);
			mod = (mod == 0) ? S + W : mod;
			assert(mod <= S + W);
//cout << x << ", " << y << ", " << time << " ------------------ " << mod << ", " << W << endl;
			if (mod <= W)
				return 1;
			return mod - W + 1;
		} else {
			int mod = (time - T) % (S + W);
			if (mod >= S)
				return 1;
			return S - mod + 1;
		}
	}
}

int main() {
	int num_test_cases; cin >> num_test_cases;
	for (int test_case = 1; test_case <= num_test_cases; test_case++) {
		int N, M; cin >> N >> M;
		int S, W, T;
		Inter E;
		vector<vector<vector<Inter> > > table;
		table.resize(N);
		for (int i = 0; i < N; i++) {
			table[i].resize(M);
			for (int j = 0; j < M; j++) {
				table[i][j].resize(4);
				cin >> E.S >> E.W >> E.T;
				E.time = -1;
				for (int corner = 0; corner < 4; corner++) {
					table[i][j][corner] = E;
				}
			}
		}
		// get answer
		int CAP = 1024*256;
		list<Pair> bfs;
		assert(table.size() == N);
		Pair p;
		p.x = N - 1; p.y = 0; p.corner = 2; p.time = 0;
		bfs.push_back(p);
		while (!bfs.empty()) {
			assert(bfs.size() < 100000);
			p = bfs.front();
			bfs.pop_front();
			// check if any good
			assert(p.x >= 0 && p.x < N);
			assert(p.y >= 0 && p.y < M);
			if (table[p.x][p.y][p.corner].time <= p.time && table[p.x][p.y][p.corner].time != -1)
				continue;
			table[p.x][p.y][p.corner].time = p.time;
			// continue on
			// go up
			Pair p_next;
			p_next.x = -1; p_next.y = -1;
			if (p.corner == 0 || p.corner == 1) {
				if (p.x > 0) {
					p_next.x = p.x - 1;
					p_next.y = p.y;
					p_next.time = p.time + 2;
					p_next.corner = (p.corner == 0) ? 3 : 2;
				}
			} else if (p.corner == 2 || p.corner == 3) {
				p_next.x = p.x;
				p_next.y = p.y;
				int time_needed = time_needed_for_intersection(table, p.x, p.y, p.time, true);
				p_next.time = p.time + time_needed;
				p_next.corner = (p.corner == 2) ? 1 : 0;
			} else
				assert(false);
			if (p_next.x != -1 && p_next.y != -1) {
				if (p_next.time < table[p_next.x][p_next.y][p_next.corner].time || table[p_next.x][p_next.y][p_next.corner].time == -1)
					bfs.push_back(p_next);
			}
			// go down
			p_next.x = -1; p_next.y = -1;
			if (p.corner == 2 || p.corner == 3) {
				if (p.x < N - 1) {
					p_next.x = p.x + 1;
					p_next.y = p.y;
					p_next.time = p.time + 2;
					p_next.corner = (p.corner == 2) ? 1 : 0;
				}
			} else if (p.corner == 0 || p.corner == 1) {
				p_next.x = p.x;
				p_next.y = p.y;
				int time_needed = time_needed_for_intersection(table, p.x, p.y, p.time, true);
				p_next.time = p.time + time_needed;
				p_next.corner = (p.corner == 0) ? 3 : 2;
			} else
				assert(false);
			if (p_next.x != -1 && p_next.y != -1) {
				if (p_next.time < table[p_next.x][p_next.y][p_next.corner].time || table[p_next.x][p_next.y][p_next.corner].time == -1)
					bfs.push_back(p_next);
			}
			// go left
			p_next.x = -1; p_next.y = -1;
			if (p.corner == 1 || p.corner == 2) {
				if (p.y > 0) {
					p_next.x = p.x;
					p_next.y = p.y - 1;
					p_next.time = p.time + 2;
					p_next.corner = (p.corner == 1) ? 0 : 3;
				}
			} else if (p.corner == 0 || p.corner == 3) {
				p_next.x = p.x;
				p_next.y = p.y;
				int time_needed = time_needed_for_intersection(table, p.x, p.y, p.time, false);
				p_next.time = p.time + time_needed;
				p_next.corner = (p.corner == 0) ? 1 : 2;
			} else
				assert(false);
			if (p_next.x != -1 && p_next.y != -1) {
				if (p_next.time < table[p_next.x][p_next.y][p_next.corner].time || table[p_next.x][p_next.y][p_next.corner].time == -1)
					bfs.push_back(p_next);
			}
			// go right
			p_next.x = -1; p_next.y = -1;
			if (p.corner == 0 || p.corner == 3) {
				if (p.y < M - 1) {
					p_next.x = p.x;
					p_next.y = p.y + 1;
					p_next.time = p.time + 2;
					p_next.corner = (p.corner == 0) ? 1 : 2;
				}
			} else if (p.corner == 1 || p.corner == 2) {
				p_next.x = p.x;
				p_next.y = p.y;
				int time_needed = time_needed_for_intersection(table, p.x, p.y, p.time, false);
//cout << p.x << ", " << p.y << ", " << p.time << ", " << p.corner << ": " << time_needed << endl;
				p_next.time = p.time + time_needed;
				p_next.corner = (p.corner == 1) ? 0 : 3;
			} else
				assert(false);
			if (p_next.x != -1 && p_next.y != -1) {
				if (p_next.time < table[p_next.x][p_next.y][p_next.corner].time || table[p_next.x][p_next.y][p_next.corner].time == -1) {
					bfs.push_back(p_next);
				}
			}
		}
		// print answer
		int answer = table[0][M - 1][0].time;
		cout << "Case #" << test_case << ": " << answer << endl;
		//for (int i = 0; i < table.size(); i++)
		//	for (int j = 0; j < table[i].size(); j++)
		//		for (int k = 0; k < table[i][j].size(); k++)
		//			cout << i << ", " << j << ", " << k << ": " << table[i][j][k].time << endl;
	}
}

