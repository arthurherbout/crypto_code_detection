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

int main(int argc, char* argv[])
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	ios_base::sync_with_stdio(false);
	srand(13);

	int T;
	cin >> T;

	vector<string> words = { "ZERO", "ONE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE" };
	vector<int> order = { 0, 2, 4, 8, 6, 1, 3, 5, 7, 9 };

	for (int test = 1; test <= T; ++test)
	{
		string s;
		cin >> s;

		map<char, int> str;
		for (char c : s)
			++str[c];

		map<int, int> answer;

		// OZONETOWER - ZERO, ONE, TWO

		for (int i = 0; i < 10; ++i)
		{
			int index = order[i];

			int count = s.size();
			for (int j = 0; j < words[index].size(); ++j)
			{
				if (str.find(words[index][j]) != str.end())
				{
					count = min(count, str[words[index][j]]);
				}
				else
				{
					count = 0;
					break;
				}
			}

			if (count)
			{
				for (int j = 0; j < words[index].size(); ++j)
				{
					if (str.find(words[index][j]) != str.end())
					{
						str[words[index][j]] -= count;
					}
				}
				answer[index] = count;
			}
		}

		cout << "Case #" << test << ": ";
		for (const auto &p : answer)
			for (int i = 0; i < p.second; ++i)
				cout << p.first;
		cout << endl;
	}

	return 0;
}

