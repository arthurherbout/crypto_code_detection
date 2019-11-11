#include <iostream>
#include <vector>

std::vector<int> search2(int sum, 
                         const std::vector<int> &set,
                         const std::vector<bool> &used,
                         const std::vector<int> &curSet,
                         int curPos,
                         int curSum) {
	if(curSum == sum) {
		return curSet;
	}

	if(curPos >= set.size()) {
		return std::vector<int>();
	}

	if(!used[curPos] && curSum + set[curPos] <= sum) {
		std::vector<int> nextSet = curSet;
		nextSet.push_back(set[curPos]);
		std::vector<int> res = search2(sum, set, used, nextSet, curPos + 1, curSum + set[curPos]);
		if(res.size() > 0) {
			return res;
		}
	}
	return search2(sum, set, used, curSet, curPos + 1, curSum);
}

std::pair<std::vector<int>, std::vector<int> > search1(int sum, 
                                                       const std::vector<int> &set,
                                                       const std::vector<bool> &used,
                                                       const std::vector<int> &curSet,
                                                       int curPos,
                                                       int curSum) {
	if(curSum == sum) {
		std::vector<int> second = search2(sum, set, used, std::vector<int>(), 0, 0);
		if(second.size() != 0) {
			return std::make_pair(curSet, second);
		}
	}

	if(curPos >= set.size()) {
		return std::make_pair(std::vector<int>(), std::vector<int>());
	}

	if(curSum + set[curPos] <= sum) {
		std::vector<bool> nextUsed = used;
		nextUsed[curPos] = true;
		std::vector<int> nextSet = curSet;
		nextSet.push_back(set[curPos]);
		std::pair<std::vector<int>, std::vector<int> > res = search1(sum, set, nextUsed, nextSet, curPos + 1, curSum + set[curPos]);
		if(res.first.size() > 0) {
			return res;
		}
	}
	return search1(sum, set, used, curSet, curPos + 1, curSum);
}

int main(int argc, char **argv) {
	int numTestCases;
	std::cin >> numTestCases;
	for(int testCaseNum = 0; testCaseNum < numTestCases; testCaseNum++) {
		int n;
		std::cin >> n;

		std::vector<int> set;
		int max = 0;
		for(int i = 0; i < n; i++) {
			int elem;
			std::cin >> elem;
			set.push_back(elem);
			max += elem;
		}

		max /= 2;
		bool succ = false;
		for(int sum = 0; sum <= max; sum++) {
			std::pair<std::vector<int>, std::vector<int> > ans = search1(sum, set, std::vector<bool>(set.size(), false), std::vector<int>(), 0, 0);
			if(ans.first.size() != 0) {
				std::cout << "Case #" << testCaseNum + 1 << ":" << std::endl;
				for(int i = 0; i < ans.first.size(); i++) {
					if(i != 0) {
						std::cout << " ";
					}
					std::cout << ans.first[i];
				}
				std::cout << std::endl;
				for(int i = 0; i < ans.second.size(); i++) {
					if(i != 0) {
						std::cout << " ";
					}
					std::cout << ans.second[i];
				}
				std::cout << std::endl;
				succ = true;
				break;
			}
		}

		if(!succ) {
			std::cout << "Impossible" << std::endl;
		}
	}

	return 0;
}
