#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

typedef pair<int, int> pii;

int main() {
	ifstream fin("B-large.in");
	ofstream fout("B-large.out");
	int n;
	fin >> n;
	for (int c = 0; c < n; c++) {
		int t;
		fin >> t;
		int na, nb;
		fin >> na >> nb;
		vector<pii> va(na);
		for (int i = 0; i < na; i++) {
			int h1, m1, h2, m2;
			char ch;
			fin >> h1 >> ch >> m1 >> h2 >> ch >> m2;
			va[i] = pii(60 * h1 + m1, 60 * h2 + m2);
		}
		vector<pii> vb(nb);
		for (int i = 0; i < nb; i++) {
			int h1, m1, h2, m2;
			char ch;
			fin >> h1 >> ch >> m1 >> h2 >> ch >> m2;
			vb[i] = pii(60 * h1 + m1, 60 * h2 + m2);
		}
		sort(va.begin(), va.end());
		sort(vb.begin(), vb.end());

		int resa = 0, resb = 0;
		int indexa = 0, indexb = 0;

		vector<int> waitinga, waitingb;
		while(indexa != va.size() || indexb != vb.size()) {
			if (indexb == vb.size() || (indexa != va.size() && va[indexa] < vb[indexb])) {
				if (waitinga.size() > 0 && waitinga[0] <= va[indexa].first)
					waitinga.erase(waitinga.begin());
				else
					resa++;
				waitingb.push_back(va[indexa].second + t);
				indexa++;
				sort(waitingb.begin(), waitingb.end());
			} else {
				if (waitingb.size() > 0 && waitingb[0] <= vb[indexb].first)
					waitingb.erase(waitingb.begin());
				else
					resb++;
				waitinga.push_back(vb[indexb].second + t);
				indexb++;
				sort(waitinga.begin(), waitinga.end());
			}
		}
		fout << "Case #" << c + 1 << ": " << resa << ' ' << resb << endl;
	}
	return 0;
}
