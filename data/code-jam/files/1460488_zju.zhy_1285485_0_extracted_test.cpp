#include <iostream>
#include <fstream>
#include <set>
#include <queue>

using namespace std;
ofstream fout ("output.out");
ifstream fin ("input.in");

#define MAX_N	30

int T, cases;
int H, W, D;
char matrix[MAX_N][MAX_N];

struct str_mirror {
	int h1, w1;
	int h2, w2;
	char dir;
};
struct str_mirror mirror[4];
int mirror_num;

struct str_node {
	int h, w;
};
bool operator< (const struct str_node& a, const struct str_node& b) {
	if (a.h == b.h)
		return a.w < b.w;
	else
		return a.h < b.h;
}
struct str_node people;

set<double> slopes_ul, slopes_ur, slopes_dl, slopes_dr;
bool line_u, line_d, line_l, line_r;

void mirrorGen() {
	mirror[0].h1 = 2;
	mirror[0].w1 = 2;
	mirror[0].h2 = 2;
	mirror[0].w2 = 2*W-2;
	mirror[0].dir = 'D';
	mirror[1].h1 = 2*H-2;
	mirror[1].w1 = 2;
	mirror[1].h2 = 2*H-2;
	mirror[1].w2 = 2*W-2;
	mirror[1].dir = 'U';
	mirror[2].h1 = 2;
	mirror[2].w1 = 2;
	mirror[2].h2 = 2*H-2;
	mirror[2].w2 = 2;
	mirror[2].dir = 'R';
	mirror[3].h1 = 2;
	mirror[3].w1 = 2*W-2;
	mirror[3].h2 = 2*H-2;
	mirror[3].w2 = 2*W-2;
	mirror[3].dir = 'L';
	mirror_num = 4;
}

bool insertSet(struct str_node node) {
	if ((node.h-people.h)*(node.h-people.h)+(node.w-people.w)*(node.w-people.w) > D)
		return false;
	double hh, ww;
	hh = node.h-people.h;
	ww = node.w-people.w;
	if (hh > 0) {
		if (ww > 0) {
			if (slopes_dr.find(hh/ww) != slopes_dr.end())
				return false;
			else {
				slopes_dr.insert(hh/ww);
				return true;
			}
		}
		else if (ww < 0) {
			if (slopes_dl.find(hh/ww) != slopes_dl.end())
				return false;
			else {
				slopes_dl.insert(hh/ww);
				return true;
			}
		}
		else {
			if (line_d)
				return false;
			else {
				line_d = true;
				return true;
			}
		}
	}
	else if (hh < 0) {
		if (ww > 0) {
			if (slopes_ur.find(hh/ww) != slopes_ur.end())
				return false;
			else {
				slopes_ur.insert(hh/ww);
				return true;
			}
		}
		else if (ww < 0) {
			if (slopes_ul.find(hh/ww) != slopes_ul.end())
				return false;
			else {
				slopes_ul.insert(hh/ww);
				return true;
			}
		}
		else {
			if (line_u)
				return false;
			else {
				line_u = true;
				return true;
			}
		}
	}
	else {
		if (ww > 0) {
			if (line_r)
				return false;
			else {
				line_r = true;
				return true;
			}
		}
		if (ww < 0) {
			if (line_l)
				return false;
			else {
				line_l = true;
				return true;
			}
		}
		else
			return false;
	}
}

void nodeGen() {
	slopes_ul.clear();
	slopes_ur.clear();
	slopes_dl.clear();
	slopes_dr.clear();
	line_u = line_d = line_l = line_r = false;
	queue<struct str_node> que;
	struct str_node curr, temp;
	int i;
	que.push(people);
	while (!que.empty()) {
		curr = que.front();
		que.pop();
		for (i=0; i<mirror_num; i++) {
			if (mirror[i].dir == 'D' && curr.h > mirror[i].h1) {
				temp.h = 2 * mirror[i].h1 - curr.h;
				temp.w = curr.w;
				if (insertSet(temp)) {
					que.push(temp);
				}
			}
			else if (mirror[i].dir == 'U' && curr.h < mirror[i].h1) {
				temp.h = 2 * mirror[i].h1 - curr.h;
				temp.w = curr.w;
				if (insertSet(temp)) {
					que.push(temp);
				}
			}
			else if (mirror[i].dir == 'R' && curr.w > mirror[i].w1) {
				temp.h = curr.h;
				temp.w = 2 * mirror[i].w1 - curr.w;
				if (insertSet(temp)) {
					que.push(temp);
				}
			}
			else if (mirror[i].dir == 'L' && curr.w < mirror[i].w1) {
				temp.h = curr.h;
				temp.w = 2 * mirror[i].w1 - curr.w;
				if (insertSet(temp)) {
					que.push(temp);
				}
			}
		}
	}
}

void work() {
	int result = 0;
	mirrorGen();
	nodeGen();
	result = slopes_ul.size() + slopes_ur.size() + slopes_dl.size() + slopes_dr.size();
	if (line_u) result ++;
	if (line_d) result ++;
	if (line_l) result ++;
	if (line_r) result ++;
	fout << "Case #" << cases << ": " << result << endl;
}

int main() {
	char temp[MAX_N+1];
	fin >> T;
	fin.getline(temp, sizeof(temp));
	for (cases=1; cases<=T; cases++) {
		fin >> H >> W >> D;
		fin.getline(temp, sizeof(temp));
		int i, j;
		for (i=0; i<H; i++) {
			fin.getline(temp, sizeof(temp));
			for (j=0; j<W; j++) {
				matrix[i][j] = temp[j];
				if (temp[j] == 'X') {
					people.h = 2*i + 1;
					people.w = 2*j + 1;
				}
			}
		}
		D = 4*D*D;
		cout << cases << endl;
		work();
	}
	system("pause");
	return 0;
}
