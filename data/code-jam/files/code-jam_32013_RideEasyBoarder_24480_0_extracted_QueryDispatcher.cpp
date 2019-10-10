#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

int main(int argc, char* argv[]) {
	string strbuf;
	ifstream is("A-small.in");
	ofstream os("A-small.txt");
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
		int numSearchEngines = atoi(strbuf.c_str());
		set<string> searchEngines;
		for(int i = 0; i < numSearchEngines; ++i) {
			getline(is, strbuf);
			searchEngines.insert(strbuf);
		}
		getline(is,strbuf);
		int numQueries = atoi(strbuf.c_str());
		vector<string> queries;
		for(int i = 0; i < numQueries; ++i) {
			getline(is, strbuf);
			queries.push_back(strbuf);
		}
		int numSwitches = 0;

		set<string> availableEngines(searchEngines);
		for(vector<string>::const_iterator it = queries.begin(), end = queries.end(); it != end; ++it) {
			set<string>::iterator find_it(availableEngines.find(*it));
			if(find_it != availableEngines.end()) {
				if(availableEngines.size() == 1) {
					numSwitches++;
					availableEngines = searchEngines;
					find_it = availableEngines.find(*it);
				}
				availableEngines.erase(find_it);
			}
		}

		os << "Case #" << (testCaseNumber+1) << ": " << numSwitches << endl;
	}
}