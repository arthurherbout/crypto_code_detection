#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<algorithm>
#define M 60
#define INF 1e7
#define e 1e-6
using namespace std;
pair<int, int> cal(int r, int q)
{	
	int min_v = (int)floor(q / (r*1.1)) - 1;
	int max_v = (int)ceil(q / (r*0.9)) + 1;
	while (min_v <= max_v)
	{
		if (min_v*r * 0.9  <= q + e && min_v*r * 1.1 >= q - e)
			break;
		else
			min_v++;
	}
	
	while (min_v <= max_v)
	{
		if (max_v*r * 0.9  <= q + e && max_v*r * 1.1 >= q - e)
			break;
		else
			max_v--;
	}

	if (max_v < min_v)
		return make_pair(-1, -1);
	else
		return make_pair(min_v, max_v);
}
int main(void)
{
	int T;
	cin >> T;
	for (int z = 1; z <= T; ++z)
	{
		int n, p;
		int r[M], q[M][M];
		int max_v = -1, min_v = INF;
		vector<pair<int, int> > s[M];
		cin >> n >> p;
		
		for (int i = 0; i < n ; ++i)
			cin >>r[i];
		
		for (int i = 0; i < n ; ++i)
		{
			for (int j = 0 ; j < p ; ++ j)
			{
				cin >> q[i][j];
				pair<int, int> tmp = cal(r[i], q[i][j]);
				s[i].push_back(tmp);
				min_v = min(min_v, tmp.first);
				max_v = max(max_v, tmp.second);
			}
			sort(s[i].begin(), s[i].end());
		}
		
		
		int now[M] = {0};
		long long  ans = 0;
		if (max_v > 0)
		{
			for (int i = max(min_v,1); i <= max_v;)
			{
				bool end = false, okay = true;
				
				for (int j = 0; j < n; ++j)
				{
					while(now[j] < s[j].size() && i > s[j][now[j]].second)
						now[j]++;
					if (now[j] >= s[j].size())
					{
						end = true;
						break;
					}
					if (i < s[j][now[j]].first)
						okay = false;
				}
				
			    if (end)
					break;
				
				if (okay)
				{
					ans++;
					for (int j = 0; j <n; ++j)
						now[j]++;
				}
				else
					i++;
				
			}
		}
		cout << "Case #" << z << ": " << ans << endl;
	}
}

