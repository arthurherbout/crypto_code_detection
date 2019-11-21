#include <vector>
#include <cmath>
#include <map>
#include <utility>
#include <stdint.h>
#include <iostream>
#include <string>
#include<iostream>
#include<fstream>

//std::map<char, int> abc;
//
//void insert(std::string& str, char c, bool antes) {
//	if (antes) {
//		str.insert(str.begin(), c);
//	}
//	else {
//		str.push_back(c);
//	}
//}
//
//void func1(std::string input) {
//	std::string new_str;
//	new_str.push_back(input[0]);
//	for (int i = 1; i < input.size(); i++) {
//		if (new_str[0] > input[i]) {
//			insert(new_str, input[i], false);
//		}
//		else {
//			insert(new_str, input[i], true);
//		}
//	}
//	printf("%s\n", new_str.c_str());
//}


std::vector<std::vector<int>> matrix1; 
std::vector<std::vector<int>> matrix2;

int get_low(std::vector<std::vector<int>>& input, int col, int* index0, int* index1, bool* found) {
	int low = 2501;
	int m = input.size();
	*index1 = -1;
	for (int i = 0; i < m; i++) {
		if (input[i][col] < low) {
			low = input[i][col];
			*index0 = i;
		}
		else if (input[i][col] == low) {
			*index1 = i;
		}
	}
	if (*index1 == -1) {
		*found = true;
	}
	else {
		*found = false;
	}
	return low;
}

int caseeee = 1;

int missing(std::vector<std::vector<int>> input, int M) {
	int m = input.size();
	int low, index0, index1;
	bool found;
	int onde;
	std::vector<int> vec1;
	std::vector<int> vec2;
	for (int i = 0; i < M; i++) {
		low = get_low(input, i, &index0, &index1, &found);
		if (found) {
			return i;
		}
		input.erase(input.begin() + index0);
		input.erase(input.begin() + index1 - 1);
	}
}

void func2(std::vector<std::vector<int>> input, int M) {
	int m = input.size();
	int low, index0, index1;
	bool found;
	int onde;
	std::vector<int> vec1;
	std::vector<int> vec2;
	std::vector<int> vec3;
	int miss = missing(input, M);
	for (int i = 0; i < M; i++) {
		low = get_low(input, i, &index0, &index1, &found);
		if (found) {
			//printf("Case #%d: ", caseeee++);
			//for (int j = 0; j < M; j++) {
			//	printf("%d ", input[index0][j]);
			//}
			//printf("\n");
			vec1.push_back(input[index0][miss]);
			vec2.push_back(input[index0][miss]);
			vec3 = input[index0];
			input.erase(input.begin() + index0);
			onde = i;
		}
		else {
			vec1.push_back(input[index0][miss]);
			vec2.push_back(input[index1][miss]);
			input.erase(input.begin() + index0);
			input.erase(input.begin() + index1 - 1);
		}
	}

	printf("Case #%d: ", caseeee++);
	if (std::equal(vec1.begin(), vec1.end(), vec3.begin())) {
		for (int i = 0; i < M; i++) {
			printf("%d ", vec2[i]);
		}
	}
	else {
		for (int i = 0; i < M; i++) {
			printf("%d ", vec2[i]);
		}
	}


	printf("\n");
}



int main(int argc, const char * argv[]) {

	std::ifstream myfile("input.txt");
	int buffer;
	int m, cases;
	myfile >> cases;

	for (int i = 0; i < cases; i++) {
		myfile >> m;
		std::vector<std::vector<int>> input;
		for (int j = 0; j < 2 * m - 1; j++) {
			input.push_back(std::vector<int>());
			for (int i = 0; i < m; i++) {
				myfile >> buffer;
				input[j].push_back(buffer);
			}
		}
		func2(input, m);
	}

	//while (!myfile.eof()) {
	//	myfile >> buffer;
	//	printf("Case #%d: ", i++);
	//	func1(buffer);
	//}

	return 0;
}