#include <iostream>
#include <set>
#include <vector>

template<class T>
int len(const T& cont) {
	return (int) cont.size();
}

std::vector<int> intToDigits(unsigned long number) {
	std::vector<int> digits;
	
	while(number) {
		digits.push_back(number % 10);
		number /= 10;
	}
	
	return std::vector<int>(digits.rbegin(), digits.rend());
}

unsigned long digitsToInt(const std::vector<int> &digits) {
	unsigned long ret = 0;
	for(int i = 0; i < digits.size(); i++) {
		ret *= 10;
		ret += digits[i];
	}
	return ret;
}

int main(int arc, char **argv) {
	int numTestCases;
	std::cin >> numTestCases;
	for(int testCaseNum = 0; testCaseNum < numTestCases; testCaseNum++) {

		int a, b;
		std::cin >> a >> b;

		int numDigits = len(intToDigits(b));

		long numPairs = 0;

		for(int i = a; i <= b; i++) {
			std::vector<int> digits = intToDigits(i);
			while(len(digits) < numDigits) {
				digits.insert(digits.begin(), 0);
			}

			std::set<int> seen;
			for(int j = 0; j < len(digits) - 1; j++) {
				int back = digits[len(digits) - 1];
				digits.pop_back();
				digits.insert(digits.begin(), back);

				int pair = digitsToInt(digits);
				if(pair >= a && pair <= b && pair > i && !seen.count(pair)) {
					seen.insert(pair);
					numPairs++;
				}
			}
		}

		std::cout << "Case #" << testCaseNum + 1 << ": " << numPairs << std::endl;
	}

	
	return 0;
}
