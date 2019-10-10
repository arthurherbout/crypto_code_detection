#include <vector>
#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <string>
#include <queue>

using namespace std;

typedef long long ll;
typedef pair<int, int> p;
fstream in("A-large.in");
fstream out("out.txt");

struct sort_pred {
	bool operator()(const std::pair<int, int> &left, const std::pair<int, int> &right) {
		return left.second > right.second;
	}
};

int main() {

	int num;
	in >> num;
	for (int test = 1; test <= num; test++){
		int n;
		in >> n;
		vector<p> vec(n);
		int total = 0;
		for (int i = 0; i < n; ++i) {
			int t;
			in >> t;
			total += t;
			vec.push_back(p(i, t));
		}
		out << "Case #" << test << ":";
		while (total > 0) {
			sort(vec.begin(), vec.end(), sort_pred());
			if (vec[0].second > vec[1].second || total == 3) {
				vec[0].second--;
				out <<" "<< char('A' + vec[0].first);
				total--;
				continue;
			}
			if (vec[0].second == vec[1].second) {
				vec[0].second--;
				vec[1].second--;
				total -= 2;
				out << " " << char('A' + vec[0].first) << char('A'+vec[1].first);
			}
		}
		out << endl;
	}
}