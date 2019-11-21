#include <iostream>

bool canMake(int goal, int total, bool special) {
	int min = goal + 2 * std::max(goal - (special ? 2 : 1), 0);

	return total >= min;
}

int maxScore(int total, bool special) {
	for(int i = 10; i >= 0; i--) {
		if(canMake(i, total, special)) {
			return i;
		}
	}
}

int main(int arc, char **argv) {
	int numTestCases;
	std::cin >> numTestCases;
	for(int testCaseNum = 0; testCaseNum < numTestCases; testCaseNum++) {
		int n, s, p;
		std::cin >> n >> s >> p;
		
		int count = 0;
		for(int i = 0; i < n; i++) {
			int total;
			std::cin >> total;

			if(maxScore(total, false) >= p) {
				count++;
			} else if(s > 0 && maxScore(total, true) >= p) {
				s--;
				count++;
			}
		}

		std::cout << "Case #" << testCaseNum + 1 << ": " << count << std::endl;
	}

	return 0;
}
