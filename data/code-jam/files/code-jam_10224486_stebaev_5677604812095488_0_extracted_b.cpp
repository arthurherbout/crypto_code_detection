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

long double solve(const vector<long double> &p, int k)
{
	int n = p.size();

	vector<vector<long double>> d(k + 1, vector<long double>(k + 1, 0));
	d[0][0] = 1;
	vector<bool> used(n, false);
	for (int i = 0; i < k; i += 2)
	{
		int best_a, best_b;
		long double best_score = -1;
		for (int a = 0; a < n; ++a)
		{
			if (used[a]) continue;
			for (int b = a + 1; b < n; ++b)
			{
				if (used[b]) continue;

				for (int w = 0; w <= k; ++w)
				{
					d[i + 1][w] = d[i][w] * (1 - p[a]);
					if (w)
					{
						d[i + 1][w] += d[i][w - 1] * p[a];
					}
				}

				for (int w = 0; w <= k; ++w)
				{
					d[i + 2][w] = d[i + 1][w] * (1 - p[b]);
					if (w)
					{
						d[i + 2][w] += d[i + 1][w - 1] * p[b];
					}
				}

				double score = d[i + 2][(i + 2) / 2];

				if (best_score < 0 || score > best_score)
				{
					best_score = score;
					best_a = a;
					best_b = b;
				}
			}
		}

		for (int w = 0; w <= k; ++w)
		{
			d[i + 1][w] = d[i][w] * (1 - p[best_a]);
			if (w)
			{
				d[i + 1][w] += d[i][w - 1] * p[best_a];
			}
		}

		for (int w = 0; w <= k; ++w)
		{
			d[i + 2][w] = d[i + 1][w] * (1 - p[best_b]);
			if (w)
			{
				d[i + 2][w] += d[i + 1][w - 1] * p[best_b];
			}
		}

		used[best_a] = true;
		used[best_b] = true;
	}

	return d[k - 1][k / 2];
}

double calc_prob(const vector<long double> &p)
{
	int k = p.size();
	vector<vector<long double>> d(k + 1, vector<long double>(k + 1, 0));
	d[0][0] = 1;
	for (int i = 0; i < p.size(); ++i)
	{
		for (int w = 0; w <= k; ++w)
		{
			d[i + 1][w] = d[i][w] * (1 - p[i]);
			if (w)
			{
				d[i + 1][w] += d[i][w - 1] * p[i];
			}
		}
	}
	return d[k][k / 2];
}

long double brute(const vector<long double> &p, int k)
{
	int n = p.size();
	long double answer = 0;
	for (int state = 0; state < (1 << n); ++state)
	{
		vector<bool> used(n, false);
		int sum = 0;
		for (int i = 0; i < n && sum <= k; ++i)
		{
			if (state & (1 << i))
			{
				used[i] = true;
				sum += 1;
			}
		}

		if (sum == k)
		{
			vector<long double> prob;
			for (int i = 0; i < n; ++i)
			{
				if (used[i]) prob.push_back(p[i]);
			}

			long double candidate = calc_prob(prob);
			if (candidate > answer)
				answer = candidate;
		}
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
		int n, k;
		cin >> n >> k;
		vector<long double> p(n);
		for (auto & x : p)
			cin >> x;

		// long double answer = solve(p, k);
		long double answer = brute(p, k);
		cout << "Case #" << test << ": " << answer << endl;
	}

	return 0;
}

