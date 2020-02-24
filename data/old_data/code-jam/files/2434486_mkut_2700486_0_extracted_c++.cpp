#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

int main();

class solver
{
private:
	int N, X, Y;

public:
	solver(int N, int X, int Y) : N(N), X(X), Y(Y) {}

	double solve()
	{
		return fall(N, 0);
	}

private:
	double fall(int n, int k)
	{
		if (abs(X) + abs(Y) < 2 * k)
			return 1.0;

		if (n >= 4 * k + 1)
			return fall(n - (4 * k + 1), k + 1);

		if (abs(X) + abs(Y) > 2 * k)
			return 0.0;

		return sub(n, k);
	}

	double sub(int n, int k)
	{
		vector<vector<double> > dp(k*4 + 1, vector<double>(k*2 + 1));

		dp[0][0] = 1.0;
		for (int i = 1; i <= n; i++)
		{
			for (int j = 0; j <= k * 2; j++)
			{
				if (j > 0)
					dp[i][j] += dp[i-1][j-1] * ((i - j) == k * 2 ? 1.0 : 0.5);
				if (i-j > 0)
					dp[i][j] += dp[i-1][j] * (j == k * 2 ? 1.0 : 0.5);
			}
		}

/*
		for (int i = 1; i <= n; i++)
		{
			for (int j = 0; j <= k * 2; j++)
			{
				cerr << dp[i][j] << " ";
			}
			cerr << endl;
		}
*/

		double ret = 0;
		if (X < 0)
			for (int i = 2 * k - abs(X) + 1; i <= k * 2; i++)
				ret += dp[n][i];
		else if (X > 0)
			for (int i = 2 * k - X + 1; i <= k * 2; i++)
				ret += dp[n][n - i];

		return ret;

	}

};

int main() {
	int T; cin >> T;
	for (int caseNo = 1; caseNo <= T; caseNo++)
	{
		int N, X, Y; cin >> N >> X >> Y;
		solver prob(N, X, Y);

		double ans = prob.solve();
		if (ans < 1e-8) ans = 0;

		cout << "Case #" << caseNo << ": " << ans << endl;
	}
	return 0;
}
