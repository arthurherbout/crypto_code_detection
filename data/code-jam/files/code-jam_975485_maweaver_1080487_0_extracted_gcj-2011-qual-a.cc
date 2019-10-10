#include <iostream>
#include <queue>
#include <vector>

int main(int arc, char **argv) {
	int numTestCases;
	std::cin >> numTestCases;

	for(int testCaseNum = 0; testCaseNum < numTestCases; testCaseNum++) {
		std::deque<int> orange;
		std::deque<int> blue;
		std::deque<bool> orangeNext;
		int numPresses;
		std::cin >> numPresses;

		for(int i = 0; i < numPresses; i++) {
			std::string robotName;
			int buttonNum;
			std::cin >> robotName >> buttonNum;

			if(robotName == "O") {
				orange.push_back(buttonNum);
				orangeNext.push_back(true);
			} else {
				blue.push_back(buttonNum);
				orangeNext.push_back(false);
			}
		}

#ifdef DEBUG
		std::cout << "Orange: [ ";
		for(std::deque<int>::const_iterator orangeIt = orange.begin();
		    orangeIt != orange.end(); ++orangeIt) {
			std::cout << *orangeIt << " ";
		}
		std::cout << "]" << std::endl;
		std::cout << "Blue: [ ";
		for(std::deque<int>::const_iterator blueIt = blue.begin();
		    blueIt != blue.end(); ++blueIt) {
			std::cout << *blueIt << " ";
		}
		std::cout << "]" << std::endl;
		std::cout << "Press order: [ ";
		for(std::deque<bool>::const_iterator orangeNextIt = orangeNext.begin();
		    orangeNextIt != orangeNext.end(); ++orangeNextIt) {
			std::cout << (*orangeNextIt ? "O" : "B") << " ";
		}
		std::cout << "]" << std::endl;
#endif

		int orangePos = 1;
		int bluePos = 1;
		int numTurns = 0;
		while(!orangeNext.empty()) {
			bool canPress = true;
			if(!orange.empty() && orangePos < orange.front()) {
#ifdef DEBUG
				std::cout << "Orange moving right from " << orangePos << " to " << orangePos + 1 << std::endl;
#endif
				orangePos++;
			} else if(!orange.empty() && orangePos > orange.front()) {
#ifdef DEBUG
				std::cout << "Orange moving left from " << orangePos << " to " << orangePos - 1 << std::endl;
#endif
				orangePos--;
			} else if(!orange.empty() && orangePos == orange.front() &&
			          !orangeNext.empty() && orangeNext.front()) {
#ifdef DEBUG
				std::cout << "Orange pressing" << std::endl;
#endif
				orangeNext.pop_front();
				orange.pop_front();
				canPress = false;
			}

			if(!blue.empty() && bluePos < blue.front()) {
#ifdef DEBUG
				std::cout << "Blue moving right from " << bluePos << " to " << bluePos + 1 << std::endl;
#endif
				bluePos++;
			} else if(!blue.empty() && bluePos > blue.front()) {
#ifdef DEBUG
				std::cout << "Blue moving left from " << bluePos << " to " << bluePos - 1 << std::endl;
#endif
				bluePos--;
			} else if(canPress && 
			          !blue.empty() && bluePos == blue.front() &&
			          !orangeNext.empty() && !orangeNext.front()) {
#ifdef DEBUG
				std::cout << "Blue pressing" << std::endl;
#endif
				orangeNext.pop_front();
				blue.pop_front();
			}

			numTurns++;
		}

		std::cout << "Case #" << testCaseNum + 1 << ": " << numTurns << std::endl;
	}

	return 0;
}
