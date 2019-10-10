#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {

	freopen("inputc.txt.in", "r", stdin);
	freopen("output.txt", "w", stdout);

	int t; cin >> t;
	for (int e=1; e<=t; e++) {
		int n,l,h; cin >> n >> l >> h;

		int s[111];
		for (int i=0; i<n; i++)
			cin >> s[i];

		int find = -1;

		printf("Case #%d: ", e);
		bool can1 = false;
		for (int i=l; i<=h; i++) {
			bool can = true;
			for (int u=0; u<n; u++) {
				if ( ! (s[u] % i == 0 || i % s[u] == 0)) {
					can = false;
					break;
				}
			}

			if (can) {
				can1 = true;
				printf("%d\n", i);
				break;
			}
		}

		if (!can1) {
			printf("NO\n");
		}		
	}	

	return 0;
}
