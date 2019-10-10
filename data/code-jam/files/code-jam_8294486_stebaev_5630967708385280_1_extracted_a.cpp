#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

int main(int argc, char* argv[])
{
	srand(13);
	ios_base::sync_with_stdio(false);

	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int T;
	cin >> T;
	for (int test = 1; test <= T; ++test) {
		int n, d;
		cin >> d >> n;
		vector<pair<int, int>> houses(n);
		for (auto &h : houses) cin >> h.first >> h.second;
		sort(houses.begin(), houses.end());

		long double time = static_cast<long double>(d - houses.back().first) / houses.back().second;
		for (int i = n - 2; i >= 0; --i)
		{
			time = max(static_cast<long double>(d - houses[i].first) / houses[i].second, time);
		}

		cout << "Case #" << test << ": ";
		cout << fixed << setprecision(8) << d / time;
		cout  << '\n';
		
	}

	return 0;
}

