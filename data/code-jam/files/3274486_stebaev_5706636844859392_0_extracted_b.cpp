#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <algorithm>
#include <sstream>
#include <time.h>

using namespace std;

const int total_minutes = 60 * 24;
// const int total_minutes = 10;

int solve(const vector<vector<bool>> &allowed, int first_person)
{
	if (!allowed[0][first_person]) return -1;
	vector<vector<vector<int>>> dynamic(total_minutes, vector<vector<int>>(total_minutes / 2 + 1, vector<int>(2, -1)));
	vector<int> activity_index(2, 0);

	dynamic[0][(first_person == 0) ? 1 : 0][first_person] = 0;

	for (int time = 1; time < total_minutes; ++time)
	{
		for (int first_duty = max(0, time + 1 - total_minutes / 2); first_duty <= min(time + 1, total_minutes / 2); ++first_duty)
		{
			for (int person = 0; person < 2; ++person)
			{
				
				if (!allowed[time][person])
				{
					continue;
				}
				if (first_duty == 0 && person == 0)
				{
					continue;
				}

				int candidate1 = -1;
				if (allowed[time - 1][person])
				{
					candidate1 = dynamic[time - 1][first_duty - ((person == 0) ? 1 : 0)][person];
				}

				int candidate2 = -1;
				int other_person = (person + 1) % 2;
				if (allowed[time - 1][other_person])
				{
					candidate2 = dynamic[time - 1][first_duty - ((person == 0) ? 1 : 0)][other_person];
					if (candidate2 >= 0)
						candidate2 += 1;
				}

				if (candidate1 == -1)
				{
					candidate1 = candidate2;
				}
				else {
					if (candidate2 != -1)
					{
						candidate1 = min(candidate1, candidate2);
					}
				}

				dynamic[time][first_duty][person] = candidate1;
			}
		}
	}

	int answer = -1;
	int candidate1 = -1;
	int candidate2 = -1;
	if (dynamic.back().back()[0] >= 0)
	{
		candidate1 = dynamic.back().back()[0] + ((first_person == 1) ? 1 : 0);
	}
	if (dynamic.back().back()[1] >= 0)
	{
		candidate2 = dynamic.back().back()[1] + ((first_person == 0) ? 1 : 0);
	}

	if (candidate1 == -1)
	{
		answer = candidate2;
	}
	else {
		if (candidate2 == -1)
		{
			answer = candidate1;
		}
		else {
			answer = min(candidate1, candidate2);
		}
	}

	return answer;
}

int solve(const vector<vector<pair<int, int>>> &activities)
{
	vector<vector<bool>> allowed(total_minutes, vector<bool>(2, true));
	for (int person = 0; person < 2; ++person)
	{
		for (auto &activity : activities[person])
		{
			for (int t = activity.first; t < activity.second; ++t)
			{
				allowed[t][person] = false;
			}
		}
	}

	int answer1 = solve(allowed, 0);
	int answer2 = solve(allowed, 1);
	if (answer1 == -1)
		return answer2;
	if (answer2 == -1)
		return answer1;
	return min(answer1, answer2);
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
		int n1, n2;
		cin >> n1 >> n2;
		vector<vector<pair<int, int>>> activities(2);
		activities[0] = vector<pair<int, int>>(n1);
		activities[1] = vector<pair<int, int>>(n2);
		for (auto &v : activities)
		{
			for (auto &a : v)
				cin >> a.first >> a.second;
			sort(v.begin(), v.end());
		}
		
		double begin = clock();
		cout << "Case #" << test << ": " << solve(activities) << '\n';
		double end = clock();
		double time = (end - begin) / CLOCKS_PER_SEC;
		cerr << "Test #" << test << ": " << time << "sec" << endl;
	}

	return 0;
}

