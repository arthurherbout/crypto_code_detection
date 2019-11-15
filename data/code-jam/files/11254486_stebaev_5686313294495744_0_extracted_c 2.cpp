#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

long long solve(const vector<vector<string>> titles)
{
	int answer = 0;
	int n = titles.size();
	for (int state = 0; state < (1 << n); ++state)
	{
		vector<bool> mask(n);
		for (int i = 0; i < n; ++i)
			mask[i] = state & (1 << i);

		set<string> good_left, good_right;
		for (int i = 0; i < n; ++i)
		{
			if (!mask[i])
			{
				good_left.insert(titles[i][0]);
				good_right.insert(titles[i][1]);
			}
		}

		bool can_be = true;
		int counter = 0;
		for (int i = 0; can_be && i < n; ++i)
		{
			if (mask[i])
			{
				can_be = good_left.find(titles[i][0]) != good_left.end() && good_right.find(titles[i][1]) != good_right.end();
				++counter;
			}
		}

		if (can_be)
			answer = max(answer, counter);
	}
	return answer;
}

int main(int argc, char* argv[])
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	ios_base::sync_with_stdio(false);
	srand(13);

	int T;
	cin >> T;
	for (int test = 1; test <= T; ++test)
	{
		int n;
		cin >> n;
		vector<vector<string>> titles(n, vector<string>(2));
		for (int i = 0; i < n; ++i)
		{
			cin >> titles[i][0] >> titles[i][1];
		}

		cout << "Case #" << test << ": " << solve(titles) << endl;
	}

	return 0;
}
