#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

const long double pi = acos(static_cast<long double>(-1));

long double surface(long long r, long long h)
{
	return h * 2 * pi * r;
}

int main(int argc, char* argv[])
{
	srand(13);
	ios_base::sync_with_stdio(false);

	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int T;
	cin >> T;
	for (int test = 1; test <= T; ++test) {
		int n, k;
		cin >> n >> k;
		vector<pair<long long, long long>> mas(n);
		for (auto &p : mas)
		{
			cin >> p.first >> p.second;
		}

		sort(mas.begin(), mas.end(), [](const pair<long long, long long> &a, const pair<long long, long long> &b)
		{
			return surface(a.first, a.second) > surface(b.first, b.second);
		});

		long double answer = 0;
		for (int i = 0; i < n; ++i)
		{
			pair<long long, long long> bottom = mas[i];
			vector<pair<long long, long long>> other;
			for (int j = 0; j < n; ++j)
			{
				if (i != j && bottom.first >= mas[j].first)
				{
					other.push_back(mas[j]);
				}
			}
			if (other.size() >= k - 1)
			{
				long double _answer = bottom.first * bottom.first * pi + surface(bottom.first, bottom.second);
				for (int j = 0; j < k - 1; ++j)
					_answer += surface(other[j].first, other[j].second);
				answer = max(answer, _answer);
			}
		}

		cout << "Case #" << test << ": ";
		cout << fixed << setprecision(9) << answer;
		cout  << '\n';
		
	}

	return 0;
}

