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

long long s[1111111];

long long ss[1111111];

int main() {

	freopen("inputl.txt.in", "r", stdin);
	freopen("output.txt", "w", stdout);

	int t; cin >> t;
	for (int e=1; e<=t; e++) {
		long long L, t, n, c;
		cin >> L >> t >> n >> c;

		long long total = 0;
		for (int i=0; i<c; i++) {
			cin >> s[i];
			//s[i] <<= 1;
			total += s[i];
		}

		for (int i=c; i<n; i++) {
			s[i] = s[i-c];
			total += s[i];
		}

		long long sum = 0;

		total *= 2;
		for (int i=0; i<n; i++) {
			if (sum + s[i] * 2 > t) {
				int end = 0;
				ss[end++] = s[i]*2 - (t - sum);
				for (int u=i+1; u<n; u++)
					ss[end++] = s[u] * 2;

				sort(ss, ss+end, greater<long long>());
				for (int u=0; u<min((long long)end, L); u++) {
					total -= ss[u] / 2;
				}

				break;

			} else {
				sum += s[i] * 2;
			}
		}

		printf("Case #%d: %lld\n", e, total);
	}	

	return 0;
}
