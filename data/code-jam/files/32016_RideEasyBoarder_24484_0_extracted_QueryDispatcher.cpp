#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <sstream>

using namespace std;

int main(int argc, char* argv[]) {
	string strbuf;
	ifstream is("A-small.in");
	ofstream os("ANSWERS.txt");
	if(!is.is_open()) {
		cerr << "Couldn't open file!" << endl;
		return -1;
	}
	if(!os.is_open()) {
		cerr << "Couldn't open file for write!" << endl;
		return -1;
	}
	getline(is, strbuf);
	int numTestCases = atoi(strbuf.c_str());
	for(int testCaseNumber = 0; testCaseNumber < numTestCases; ++testCaseNumber) {
		getline(is, strbuf);
		int numElements = atoi(strbuf.c_str());
		vector<int> firstVector;
		
		getline(is, strbuf);
		stringstream ss1(strbuf);

		for(int i = 0; i < numElements; ++i) {
			ss1 >> strbuf;
			firstVector.push_back(atoi(strbuf.c_str()));
		}
		
		getline(is, strbuf);
		
		stringstream ss2(strbuf);
		vector<int> secondVector;
		for(int i = 0; i < numElements; ++i) {
			ss2 >> strbuf;
			secondVector.push_back(atoi(strbuf.c_str()));
		}
		
		sort(firstVector.begin(), firstVector.end());
		sort(secondVector.rbegin(), secondVector.rend());

		int sum = 0;
		for(vector<int>::const_iterator it1(firstVector.begin()), it2(secondVector.begin()), end1(firstVector.end()); it1 != end1; ++it1) {
			sum += (*it1) * (*it2);
			++it2;
		}

		os << "Case #" << (testCaseNumber+1) << ": " << sum << endl;
	}
}