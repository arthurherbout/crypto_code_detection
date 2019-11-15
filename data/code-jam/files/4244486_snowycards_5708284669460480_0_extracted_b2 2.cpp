#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <map>

using namespace std;

#define rep(i,n) for(int i = 0, _##i = n; i < _##i; ++i)
#define dwn(i,n) for(int i = n; i >= 0; --i)
#define itr(v, it) for(__typeof(v.begin()) it = v.begin(); it != v.end(); it++)

#define vi vector<int>
#define max(a,b) (a>b?a:b)
#define pb push_back
#define ll long long

#define INF (int) 1e9
#define EPS 1e-9

void generateCombinations(vector<string> &v, string current, string base, int endLength) {
	if(current.size() == endLength) {
		v.pb(current);
		return;
	}

	for(int i = 0; i < base.size(); i++) {
		string* newCurrent = new string(current + base[i]);
		generateCombinations(v, *newCurrent, base, endLength);
	}
}

void solve() {

	int K, L, S;

	cin >> K >> L >> S;

	string charsAvailable;
	cin >> charsAvailable;

	string targetWord;
	cin >> targetWord;

	vector<string> v;
	generateCombinations(v, "", charsAvailable, S);


	double numMatches = 0;
	double maxMatches = 0;
	double localMatches = 0;
	for(int i = 0; i < v.size(); i++) {
		localMatches = 0;
		for(int j = 0; j < v[i].size()-targetWord.size()+1; j++) {
			for(int k = 0; k < targetWord.size(); k++) {
				if(v[i][j+k] != targetWord[k])
					break;
				if(k == targetWord.size()-1) {
					localMatches++;
					numMatches++;
					//cout << "found " << targetWord << " in " << v[i] << endl;
				}
			}
		}
		maxMatches = max(maxMatches, localMatches);
	}
	//printf("maxMatches: %f\n", maxMatches);
	printf("%f\n", maxMatches - numMatches/v.size());
}

int main() {

	int cases;
	cin >> cases;

	rep(current, cases) {

		printf("Case #%d: ", current+1);
		solve();
	}

	return 0;
}