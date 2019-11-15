#include <iostream>
#include <string>
#include <sstream>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <vector>
using namespace std;

int T;

int main() {
	cin >> T;
	for (int t=1; t<=T; t++) {
		int N;
		int ind = 1;
		unordered_set <int> S[20];
		unordered_map <string, int> MI;
		string s, s1;
		cin >> N;
		getline(cin, s);
		for (int i=0; i<N; i++) {
			getline(cin, s);
			stringstream ss(s);
			while (getline(ss, s1, ' ')) {
				int index = MI[s1];
				if (index == 0)
					MI[s1] = index = ind++;
				S[i].insert(index);
			}
		}
		int ans = 100000000000;
		
		for (int mask=2; mask<(1<<N); mask+=4) {
			int am = 0;
			vector <int> both(ind, 0);
			for (int e=0; e<N; e++)
				if ((mask & (1<<e)) == 0)
					for (auto &se: S[e]) if (both[se] == 0)
						for (int f=0; f<N; f++)
							if ((mask & (1<<f)) > 0)
								if (S[f].count(se) > 0) {
									both[se] = 1;
									am++;
									break;
								}
			ans = min(ans, am);
		}

		cout << "Case #" << t << ": " << ans << endl;
	}
}
