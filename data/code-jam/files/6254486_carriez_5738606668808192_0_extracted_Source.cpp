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
using namespace std;
typedef long long	ll;

uint64_t prime(uint64_t coin) {
	uint64_t dmax = sqrt(coin);
	for (uint64_t i = 2; i <= dmax; ++i) {
		if (coin %i == 0) {
			return i;
		}
	}
	return 0;
}
uint64_t calculate(uint64_t coin, int n, int base) {
	uint64_t sum = 0;
	uint64_t b = 1;
	for (int i = 0; i < n; i++) {
		if (coin & (1 << i)) {
			sum += b;
		}
		b *= base;
	}
	return sum;
}
unordered_map<string, vector<uint64_t> > helperC() {
	int n = 0, j = 0;
	cin >> n >> j;
	uint64_t mincoin = (1 << (n - 1)) + 1;
	uint64_t maxcoin = (1 << n) - 1;
	int cnt = 0;
	unordered_map<string, vector<uint64_t> > ans;
	vector<uint64_t> dvec(9, 0);
	for (uint64_t coin = mincoin; cnt<j && coin <= maxcoin; coin += 2) {
		int i = 2;
		for (i = 2; i <= 10; ++i) {
			uint64_t jamcoin = calculate(coin, n, i);
			int div = prime(jamcoin);
			if (div) {
				dvec[i-2]=div;
			}
			else {
				break;
			}
		}
		if (i == 11) {
			string str = "";
			for (int j = n - 1; j >= 0; j--) {
				if (coin & (1 << j)) {
					str.push_back('1');
				}
				else {
					str.push_back('0');
				}
			}
			ans[str] = dvec;
			cnt++;
		}
	}
	return ans;
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
	freopen("C-small-attempt2.in", "r", stdin);
	freopen("C-small-attempt2.out", "w", stdout);
#endif
#ifdef LARGE
	freopen("B-large.in", "r", stdin);
	freopen("B-large.out", "w", stdout);
#endif
	int n = 0;
	cin >> n;
	string dump;
	getline(cin, dump);
	//bitset<128> s;
	for (int i = 1; i <= n; i++) {
		printf("Case #%d: ", i);
		printf("\n");
		unordered_map<string, vector<uint64_t> > ans = helperC();
		for (auto iter = ans.begin(); iter != ans.end(); iter++) {
			cout << iter->first << " ";
			for (int i = 0; i < iter->second.size(); i++) {
				cout<< iter->second[i] <<" ";
			}
			printf("\n");
		}
		cout << endl;
	}
}