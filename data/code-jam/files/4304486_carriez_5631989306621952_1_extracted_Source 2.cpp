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

string helperA() {
	string str;
	getline(cin, str);
	int n = str.size();
	list<char> ans;
	if (!n) return "";
	char lead = str[0];

	ans.push_back(lead);
	for (int i = 1; i < n; ++i) {
		if (str[i] >= lead) {
			ans.push_front(str[i]);
			lead = str[i];
		}
		else {
			ans.push_back(str[i]);
		}
	}
	string s;
	s.reserve(n);
	for (auto iter = ans.begin(); iter != ans.end(); iter++) {
		s.push_back(*iter);
	}
	return s;
}

//#define TEST
//#define SMALL
#define LARGE
int main() {
#ifdef TEST
	freopen("test.in", "r", stdin);
	freopen("test.out", "w", stdout);
#endif
#ifdef SMALL
	freopen("A-small-attempt0.in", "r", stdin);
	freopen("A-small-attempt0.out", "w", stdout);
#endif
#ifdef LARGE
	freopen("A-large.in", "r", stdin);
	freopen("A-large.out", "w", stdout);
#endif
	int n = 0;
	cin >> n;
	string dump;
	getline(cin, dump);
	for (int i = 1; i <= n; i++) {
		printf("Case #%d: ", i);
		cout << helperA();
		cout << endl;
	}
}