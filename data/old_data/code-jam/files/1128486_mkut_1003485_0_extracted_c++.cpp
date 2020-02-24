#include <iostream>
#include <vector>
using namespace std;

typedef long long int ll;

const ll INF = 1ULL<<60;
ll dp[1001][3] = {};

int main()
{
	int T; cin >> T;
	for(int No = 1; No <= T; No++)
	{
		ll L, t, N, C; cin >> L >> t >> N >> C;
		vector<ll> dist(C);
		for(int i = 0; i < C; i++)
			cin >> dist[i];
		
		//vector<vector<ll> > dp(N+1, vector<ll>(L+1, INF));
		//for(int i = 0; i <= L; i++)
		//	dp[0][i] = 0;
		for(int i = 1; i <= N; i++)
			for(int j = 0; j <= L; j++)
				dp[i][j] = INF;
		
		for(int i = 0; i < N; i++)
		{
			for(int j = 0; j <= L; j++)
			{
				dp[i+1][j] = min(dp[i+1][j], dp[i][j] + dist[i%C] * 2);
				if(j > 0)
					dp[i+1][j-1] = min(dp[i+1][j-1], dp[i][j] + dist[i%C] + max(0LL, (t - dp[i][j])/2));
			}
		}
		/*
		for(int i = 0; i <= N; i++)
		{
			for(int j = 0; j <= L; j++)
				cout << dp[i][j] << " ";
			cout << endl;
		}
		*/
		cout << "Case #" << No << ": ";
		cout << dp[N][0] << endl;
	}
	return 0;
}
