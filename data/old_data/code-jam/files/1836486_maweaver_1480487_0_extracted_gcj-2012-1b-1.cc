#include <iomanip>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
	int numTestCases;
	std::cin >> numTestCases;
	for(int testCaseNum = 0; testCaseNum < numTestCases; testCaseNum++) {
		int n;
		std::cin >> n;

		std::vector<long double> scores;
		long double total = 0;

		for(int i = 0; i < n; i++) {
			long double score;
			std::cin >> score;
			scores.push_back(score);
			total += score;
		}

		std::cout << "Case #" << testCaseNum + 1 << ":";
		for(int i = 0; i < n; i++) {
			long double pct = ((long double) 2.0 * total - scores[i] * (long double) n) / ((long double) n  * total);
			pct = std::max(pct, (long double) 0);
			std::cout << " " << std::fixed << std::setprecision(8) << 100.0 * pct;
		}
		std::cout << std::endl;
	}

	return 0;
}
