#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

vector<string> col_s;
vector<int> col;
vector<bool> first;

void genCol() {
	map<string,int> m;
	col.resize(col_s.size());
	first.resize(col_s.size());
	int nextColor = 0;
	for (unsigned int i=0; i<col_s.size(); i++) {
		if (m.find(col_s[i])==m.end()) {
			m[col_s[i]] = nextColor++;
			first[i] = true;
		} else first[i] = false;
		col[i] = m[col_s[i]];
	}
}

int getHeight(int mask) {
	int max = 0;
	vector<int> top, bot;
	for (unsigned int i=0; i<col.size(); i++) {
		if (first[i]) {
			if ((mask>>col[i])&1) top.push_back(col[i]);
			else bot.push_back(col[i]);
			if ((int)top.size()+(int)bot.size()>max)
				max = top.size() + bot.size();
		} else {
			if (!top.empty() && top.back()==col[i])
				top.pop_back();
			else if (!bot.empty() && bot.back()==col[i])
				bot.pop_back();
			else
				return 1000;
		}
	}
	return max;
}

int run() {
	genCol();
	int best = 1000;
	for (int mask=0; mask<(1<<(col.size()/2)); mask++) {
		int res = getHeight(mask);
		if (res<best)
			best = res;
	}
	return best==1000 ? -1 : best;
}

int main() {
	ifstream fi("e.in");
	ofstream fo("e.out");
	int T;
	fi >> T;
	for (int t=1; t<=T; t++) {
		int n;
		fi >> n;
		col_s.resize(2*n);
		for (int i=0; i<2*n; i++)
			fi >> col_s[i];
		fo << "Case #" << t << ": " << run() << endl;
		cout << t << '/' << T << endl;
	}
	return 0;
}

