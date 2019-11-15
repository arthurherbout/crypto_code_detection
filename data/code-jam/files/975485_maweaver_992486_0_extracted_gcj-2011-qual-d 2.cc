#include <cstdio>
#include <vector>

int main(int arc, char **argv) {
	int numTestCases;
	std::scanf("%d", &numTestCases);

	for(int testCaseNum = 0; testCaseNum < numTestCases; testCaseNum++) {
		int n;
		std::scanf("%d", &n);

		std::vector<int> numbers;
		for(int i = 0; i < n; i++) {
			int num;
			std::scanf("%d", &num);
			numbers.push_back(num);
		}

		int numCorrect = 0;
		for(int i = 0; i < n; i++) {
			if(numbers[i] == i + 1) {
				numCorrect++;
			}
		}

		std::printf("Case #%d: %f\n", testCaseNum + 1, (float) (n - numCorrect));
	}

	return 0;
}
