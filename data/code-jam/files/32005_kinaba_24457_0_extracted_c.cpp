#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>
#include <complex>
#include <cmath>
using namespace std;
typedef long long LL;

int main()
{
	int NC; cin >> NC;
	for(int ID=1; ID<=NC; ++ID)
	{
		cout << "Case #" << ID << ": ";
		cout << setiosflags(ios::fixed) << setprecision(8);

		int M; double P, X;
		cin >> M >> P >> X;

		vector<double> dp(1, 0); dp.push_back(1);
		for(int i=0; i<M; ++i)
		{
			vector<double> dp2( 1<<i+1, 0 );  dp2.push_back(1);
			for(int j=0; j<(1<<i+1); ++j)
			{
				double p = 0;
				// ...
				for(int k=0; k<=j; ++k)
				{
					double lo = dp[(j-k)/2];
					double hi = (j+k)/2 < dp.size() ? dp[(j+k)/2] : 1;
					p = max(p, P*hi + (1-P)*lo);
				}
				// ...
				dp2[j] = p;
			}
			dp.swap(dp2);
		}

		LL I = (LL(X)<<M) / 1000000;
		cout << dp[I] << endl;
	}
}
