#include <iostream>
#include <limits>

template<class T>
int len(const T& cont) {
	return (int) cont.size();
}

char table[] = {
	'y', // a
	'h', // b
	'e', // c
	's', // d
	'o', // e
	'c', // f
	'v', // g
	'x', // h
	'd', // i
	'u', // j
	'i', // k
	'g', // l
	'l', // m
	'b', // n
	'k', // o
	'r', // p
	'z', // q
	't', // r
	'n', // s
	'w', // t
	'j', // u
	'p', // v
	'f', // w
	'm', // x
	'a', // y
	'q', // z
};

int main(int arc, char **argv) {
	
	int numTestCases;
	std::cin >> numTestCases;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	for(int testCaseNum = 0; testCaseNum < numTestCases; testCaseNum++) {
		std::string line;
		std::getline(std::cin, line);

		for(int i = 0; i < len(line); i++) {
			if(line[i] != ' ') {
				line[i] = table[line[i] - 'a'];
			}
		}

		std::cout << "Case #" << testCaseNum + 1 << ": " << line << std::endl;
	}

	return 0;
}
