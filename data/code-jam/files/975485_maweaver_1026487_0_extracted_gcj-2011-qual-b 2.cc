#include <iostream>
#include <queue>
#include <vector>

int main(int arc, char **argv) {
	
	int numTestCases;
	std::cin >> numTestCases;

	for(int testCaseNum = 0; testCaseNum < numTestCases; testCaseNum++) {
		std::vector<std::vector<int> > combiners(26, std::vector<int>(26, -1));
		std::vector<std::vector<bool> > opposers(26, std::vector<bool>(26, false));

		int numCombiners;
		std::cin >> numCombiners;
		for(int i = 0; i < numCombiners; i++) {
			std::string combiner;
			std::cin >> combiner;
			int from1 = combiner[0] - 'A';
			int from2 = combiner[1] - 'A';
			int to = combiner[2] - 'A';

			combiners[from1][from2] = to;
			combiners[from2][from1] = to;
		}

		int numOpposers;
		std::cin >> numOpposers;
		for(int i = 0; i < numOpposers; i++) {
			std::string opposer;
			std::cin >> opposer;

			int opp1 = opposer[0] - 'A';
			int opp2 = opposer[1] - 'A';

			opposers[opp1][opp2] = true;
			opposers[opp2][opp1] = true;
		}

		int n;
		std::cin >> n;
		std::string line;
		std::cin >> line;

#ifdef DEBUG
		std::cout << "====================" << std::endl;
		for(int i = 0; i < 26; i++) {
			for(int j = 0; j < 26; j++) {
				std::cout.width(2);
				std::cout << combiners[i][j] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << "----------" << std::endl;
		for(int i = 0; i < 26; i++) {
			for(int j = 0; j < 26; j++) {
				std::cout << opposers[i][j] << " ";
			}
			std::cout << std::endl;
		}
#endif

		std::deque<int> queue;
		for(int i = 0; i < n; i++) {
			int ch = line[i] - 'A';
			bool combined = false;
			if(!queue.empty()) {
				int prev = queue.back();
				if(combiners[prev][ch] != -1) {
					queue.pop_back();
					queue.push_back(combiners[prev][ch]);
					combined = true;
				}
			}

			if(!combined) {
				bool erased = false;
				for(std::deque<int>::const_iterator queueIt = queue.begin();
				    queueIt != queue.end(); ++queueIt) {
				
					if(opposers[ch][*queueIt]) {
						queue = std::deque<int>();
						erased = true;
						break;
					}
				}
				if(!erased) {
					queue.push_back(ch);
				}
			}
		}

		std::cout << "Case #" << testCaseNum + 1 << ": [";
		bool first = true;
		for(std::deque<int>::const_iterator queueIt = queue.begin();
		    queueIt != queue.end(); ++queueIt) {
			
			if(!first) {
				std::cout << ", ";
			}
			first = false;
			std::cout << (char) (*queueIt + 'A');
		}
		std::cout << "]" << std::endl;
	}

	return 0;
}
