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

char getLess(char c)
{
	if (c == 'R')
	{
		return 'S';
	}
	if (c == 'S')
	{
		return 'P';
	}
	return 'R';
}

void calc(const string &order, int &p, int &r, int &s)
{
	p = 0;
	r = 0;
	s = 0;
	for (char c : order)
	{
		if (c == 'P') ++p;
		if (c == 'R') ++r;
		if (c == 'S') ++s;
	}
}

string solve(const int n, const int p, const int r, const int s, const char top = -1)
{
	if (top > 0)
	{
		if (top == 'P' && p < 1) return "";
		if (top == 'R' && r < 1) return "";
		if (top == 'S' && s < 1) return "";
	}

	if (n == 0)
	{
		if (top > 0)
		{
			return string(1, top);
		}
		else
		{
			if (p)
			{
				return "P";
			}
			if (r)
			{
				return "R";
			}
			if (s)
			{
				return "S";
			}
			return "";
		}
	}
	else
	{
		if (top > 0)
		{
			char other = getLess(top);

			if (other == 'P' && p < 1) return "";
			if (other == 'R' && r < 1) return "";
			if (other == 'S' && s < 1) return "";

			int rest_p, rest_r, rest_s;
			string left, right;
			string ans1, ans2;

			left = solve(n - 1, p, r, s, top);
			if (left.size() > 0)
			{
				calc(left, rest_p, rest_r, rest_s);
				right = solve(n - 1, p - rest_p, r - rest_r, s - rest_s, other);
				if (right.size() > 0)
				{
					ans1 = left + right;
				}
			}

			left = "";
			right = "";

			left = solve(n - 1, p, r, s, other);
			if (left.size() > 0)
			{
				calc(left, rest_p, rest_r, rest_s);
				right = solve(n - 1, p - rest_p, r - rest_r, s - rest_s, top);
				if (right.size() > 0)
				{
					ans2 = left + right;
				}
			}

			if (ans1.size() == 0) return ans2;
			if (ans2.size() == 0) return ans1;

			if (ans1 < ans2)
				return ans1;
			else
				return ans2;
		}
		else
		{
			string answer;
			for (char top : string("PRS"))
			{
				string candidate = solve(n, p, r, s, top);
				if (candidate.size() > 0)
				{
					if (answer.size() > 0)
					{
						if (candidate < answer)
						{
							answer = candidate;
						}
					}
					else
					{
						answer = candidate;
					}
				}
			}
			return answer;
		}
	}
}

//string brute(const int n, const int p, const int r, const int s)
//{
//
//}

void stress()
{
	double worst_time = 0;
	int n = 3;
	int control = 0;
	for (int counter = 1;; ++counter)
	{
		int sum = 1 << n;
		int p, r, s;
		p = rand() % (sum + 1);
		sum -= p;
		r = rand() % (sum + 1);
		sum -= r;
		s = sum;

		double start = clock();
		string answer = solve(n, p, r, s);
		double finish = clock();

		double time = (finish - start) / CLOCKS_PER_SEC;
		if (time > worst_time)
		{
			worst_time = time;
			cerr << "Worst time: " << worst_time << "sec" << endl;
		}

		if (answer.size())
		{
			int _p, _r, _s;
			calc(answer, _p, _r, _s);
			if (_p != p || _r != r || _s != s)
			{
				cout << "WA!!!" << endl;
				cout << n << ' ' << r << ' ' << p << ' ' << s << endl;
				cout << "Got: " << answer << endl;
				return;
			}
		}

		if (counter % 100 == 0) cout << "\r" << counter;
	}
	cerr << control << endl;
}

int main(int argc, char* argv[])
{
	// stress();
	// return 0;

	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	ios_base::sync_with_stdio(false);
	srand(13);

	int T;
	cin >> T;
	for (int test = 1; test <= T; ++test)
	{
		int n, r, p, s;
		cin >> n >> r >> p >> s;
		string answer = solve(n, p, r, s);
		if (answer.size() == 0)
			answer = "IMPOSSIBLE";
		cout << "Case #" << test << ": " << answer << endl;
	}

	return 0;
}

