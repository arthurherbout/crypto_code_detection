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
#include <iomanip>

using namespace std;

pair<long long, long long> solve(string::const_iterator c_begin, string::const_iterator c_end, 
	string::const_iterator j_begin, string::const_iterator j_end, 
	int cmp, long long current_power)
{
	int a, b;
	if (*c_begin != '?') a = *c_begin - '0';
	else a = -1;
	if (*j_begin != '?') b = *j_begin - '0';
	else b = -1;

	if (cmp)
	{
		if (cmp > 0)
		{
			// C is bigger

			if (a < 0) a = 0;
			if (b < 0) b = 9;
		} 
		else
		{
			if (a < 0) a = 9;
			if (b < 0) b = 0;
		}

		pair<long long, long long> answer;
		if (current_power > 1)
		{
			answer = solve(c_begin + 1, c_end, j_begin + 1, j_end, cmp, current_power / 10);
		}
		else
		{
			answer = make_pair(0, 0);
		}
		answer.first += a * current_power;
		answer.second += b * current_power;
		return answer;
	}
	else
	{
		// C prefix is equal to J prefix

		int min_a, max_a, min_b, max_b;
		bool a_and_b_is_not_ones_together = false;
		if (a >= 0)
		{
			min_a = a;
			max_a = a;
			if (b >= 0)
			{
				min_b = b;
				max_b = b;
			}
			else
			{
				min_b = max(0, min_a - 1);
				max_b = min(9, max_a + 1);
			}
		}
		else
		{
			if (b >= 0)
			{
				min_b = b;
				max_b = b;
				min_a = max(0, min_b - 1);
				max_a = min(9, max_b + 1);
			}
			else
			{
				min_a = 0;
				max_a = 1;
				min_b = 0;
				max_b = 1;
				a_and_b_is_not_ones_together = true;
			}
		}

		pair<long long, long long> answer = make_pair(-1, -1);
		for (int current_a = min_a; current_a <= max_a; ++current_a)
		{
			for (int current_b = min_b; current_b <= max_b; ++current_b)
			{
				if (a_and_b_is_not_ones_together && current_a == 1 && current_b == 1) continue;

				int cmp = 0;
				if (current_a > current_b) cmp = 1;
				if (current_a < current_b) cmp = -1;

				pair<long long, long long> candidate;
				if (current_power > 1)
				{
					candidate = solve(c_begin + 1, c_end, j_begin + 1, j_end, cmp, current_power / 10);
				}
				else
				{
					candidate = make_pair(0, 0);
				}
				candidate.first += current_a * current_power;
				candidate.second += current_b * current_power;

				if (answer.first < 0)
				{
					answer = candidate;
				}
				else
				{
					if (abs(answer.first - answer.second) > abs(candidate.first - candidate.second))
					{
						answer = candidate;
					}
					else
					{
						if (abs(answer.first - answer.second) == abs(candidate.first - candidate.second))
						{
							if (current_a < answer.first / current_power)
							{
								answer = candidate;
							}
							else
							{
								if (current_a == answer.first / current_power)
								{
									if (current_b < answer.second / current_power)
									{
										answer = candidate;
									}
								}
							}
						}
					}
				}
			}
		}
		return answer;
	}
}

pair<long long, long long> solve(const string &C, const string &J)
{
	return solve(C.begin(), C.end(), J.begin(), J.end(), 0, pow(10, C.end() - C.begin() - 1));
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
		string C, J;
		cin >> C >> J;
		auto answer = solve(C, J);
		cout << "Case #" << test << ": " << setfill('0') << setw(C.size()) << answer.first << " " << setfill('0') << setw(J.size()) << answer.second << endl;
	}

	return 0;
}
