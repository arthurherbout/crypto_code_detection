#include <iostream>
#include <vector>

using namespace std;

struct triplet {
	int jacket;
	int pants;
	int shirt;
	triplet() {}
	triplet(int j, int p, int s) { jacket = j; pants = p; shirt = s; }
};

void testcase() {
	int j,p,s,k;
	cin >> j >> p >> s >> k;

	/*vector<vector<vector<bool> > > outfit(j);
	for (int i = 0; i < j; ++i) {
		outfit[i].resize(p);
		for (int ii = 0; ii < p; ++ii) {
			outfit[i][ii].resize(s);
			for (int iii = 0; iii < s; ++iii) {
				outfit[i][ii][iii] = false;
			}
		}
		}*/

	vector<vector<int> > jp(j);
	for (int i = 0; i < j; ++i) {
	    jp[i].resize(p);
		for (int ii = 0; ii < p; ++ii) {
		    jp[i][ii] = 0;
		}
	}

	vector<vector<int> > js(j);
	for (int i = 0; i < j; ++i) {
	    js[i].resize(s);
		for (int ii = 0; ii < s; ++ii) {
		    js[i][ii] = 0;
		}
	}

	vector<vector<int> > ps(p);
	for (int i = 0; i < p; ++i) {
	    ps[i].resize(s);
		for (int ii = 0; ii < s; ++ii) {
		    ps[i][ii] = 0;
		}
	}

	vector<triplet> outfits;
	for (int i = 0; i < j; ++i) {
		for (int ii = 0; ii < p; ++ii) {
			for (int iii = 0; iii < s; ++iii) {
				if (/*!outfit[i][ii][iii] and */jp[i][ii] < k and js[i][iii] < k and ps[ii][iii] < k) {
					outfits.push_back(triplet(i+1, ii+1, iii+1));
					//outfit[i][ii][iii] = true;
					jp[i][ii]++;
					js[i][iii]++;
					ps[ii][iii]++;
				}
			}
		}
	}

	cout << outfits.size() << endl;

	for (int i = 0; i < outfits.size(); ++i) {
		cout << outfits[i].jacket << " "  << outfits[i].pants << " "  << outfits[i].shirt;
		if (i < outfits.size() - 1) cout << endl;
	}
}

int main() {
	int t;
	cin >> t;
	for (int i = 1; i <= t; ++i) {
	    cout << "Case #" << i << ": ";
	    testcase();
	    cout << endl;
	}
}
