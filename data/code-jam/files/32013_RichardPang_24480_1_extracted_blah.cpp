#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

int main() {
	ifstream fin("A-large.in");
	ofstream fout("A-large.out");
	
	int n;
	fin >> n;
	for (int t = 0; t < n; t++) {
		int ns;
		fin >> ns;
		string blank;
		getline(fin, blank);
		vector<string> vs(ns);
		for (int i = 0; i < ns; i++)
			getline(fin, vs[i]);
		int nq;
		fin >> nq;
		vector<string> vq(nq);
		getline(fin, blank);
		for (int i = 0; i < nq; i++)
			getline(fin, vq[i]);

		int res = 10000;
		if (nq > 0) {
			vector<vector<int> > best(ns, vector<int>(nq + 1, 10000));
			for (int i = 0; i < ns; i++)
				best[i][nq - 1] = (vs[i] == vq[nq - 1]);
			for (int j = nq - 2; j >= 0; j--) {
				for (int i = 0; i < ns; i++) {
					if (vs[i] == vq[j])
						best[i][j] = 10000;
					else
						best[i][j] = best[i][j + 1];
					for (int k = 0; k < ns; k++)
						if (vs[k] != vq[j])
							best[i][j] = min(best[i][j], 1 + best[k][j + 1]);
				}
			}
			for (int i = 0; i < ns; i++)
				res = min(res, best[i][0]);
		} else res = 0;
		fout << "Case #" << t + 1 << ": " << res << endl;
	}
	return 0;
}
