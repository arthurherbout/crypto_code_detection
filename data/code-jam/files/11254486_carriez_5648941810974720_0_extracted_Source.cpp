#define _CRT_SECURE_NO_WARNINGS
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<set>
#include<iostream>
#include<math.h>
#include<algorithm>
#include<string>
#include<queue>
#include<stdint.h>
#include<bitset>
#include<deque>
#include<list>
using namespace std;
typedef long long	ll;

bool check(int n, vector<int> &strvec, vector<vector<int> > &number){
	for (int i = 0; i < number[n].size(); i++){
		if (number[n][i] > strvec[i]){
			return false;
		}
	}
	return true;
}
bool dfs(vector<int> &strvec, int ind, unordered_map<char, vector<int> > &inverted, vector<vector<int> > &number, vector<int> &ans){
	if (ind > 25) return true;
	if (!strvec[ind]){
		return dfs(strvec, ind + 1, inverted, number, ans);
	}
	else{
		char c = 'A' + ind;
		for (int i = 0; i < inverted[c].size(); i++){
			int n = inverted[c][i];
			if (check(n, strvec, number)){
				for (int j = 0; j < number[n].size(); j++){
					strvec[j] -= number[n][j];
				}
				if (!dfs(strvec, ind, inverted, number, ans)){
					for (int j = 0; j < number[n].size(); j++){
						strvec[j] += number[n][j];
					}
				}
				else{
					ans.push_back(n);
					return true;
				}
			}
		}
	}
	return false;
}

string helperA(unordered_map<char, vector<int> > &inverted, vector<vector<int> > &number){
	string str;
	getline(cin, str);
	vector<int> strvec(26, 0);
	for (int i = 0; i < str.size(); i++){
		strvec[str[i] - 'A'] += 1;
	}
	vector<int> ans;
	dfs(strvec, 0, inverted, number, ans);
	sort(ans.begin(), ans.end());
	string ret;
	for (int i = 0; i < ans.size(); i++){
		ret.push_back('0' + ans[i]);
	}
	return ret;
}

//#define TEST
#define SMALL
//#define LARGE
int main() {
#ifdef TEST
	freopen("test.in", "r", stdin);
	freopen("test.out", "w", stdout);
#endif
#ifdef SMALL
	freopen("A-small-attempt2.in", "r", stdin);
	freopen("A-small-attempt2.out", "w", stdout);
#endif
#ifdef LARGE
	freopen("A-large.in", "r", stdin);
	freopen("A-large.out", "w", stdout);
#endif
	int n = 0;
	cin >> n;
	string dump;
	getline(cin, dump);

	unordered_map<char, vector<int> > inverted;
	vector<vector<int> > number(10, vector<int>(26,0));
	string num[] = { "ZERO", "ONE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE" };
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < num[i].size(); j++){
			char c = num[i][j];
			if (inverted.find(c) == inverted.end()){
				inverted[c] = vector<int>(1, i);
			}
			else{
				inverted[c].push_back(i);
			}
			number[i][c - 'A'] += 1;
		}
	}


	for (int i = 1; i <= n; i++) {
		printf("Case #%d: ", i);
		cout << helperA(inverted, number);
		cout << endl;
	}
}