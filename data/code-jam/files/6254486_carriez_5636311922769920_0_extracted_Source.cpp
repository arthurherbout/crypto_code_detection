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


//#define TEST
#define SMALL
//#define LARGE
int main() {
#ifdef TEST
	freopen("test.in", "r", stdin);
	freopen("test.out", "w", stdout);
#endif
#ifdef SMALL
	freopen("D-small-attempt0.in", "r", stdin);
	freopen("D-small-attempt0.out", "w", stdout);
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
		int k, c, s;
		cin >> k >> c >> s;
		for (int i = 1; i <=k ; ++i){
			cout << i << " ";
		}
		cout << endl;
	}
}