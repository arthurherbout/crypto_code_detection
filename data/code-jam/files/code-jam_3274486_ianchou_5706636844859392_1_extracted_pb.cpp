#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<queue>
#include<stack>
#include<cfloat>
#include<algorithm>
using namespace std;

long long int_pow(int base, int exp)
{
    long long result = 1;
    while (exp)
    {
        if (exp & 1)
           result *= base;
        exp /= 2;
        base *= base;
    }
    return result;
}

int main(void)
{
	int T;
	cin >> T;
	for (int z = 1; z <= T; ++z)
	{
		int ac, aj;
		vector<int > b, cost[2];
		map<int, pair<int, int> > info;
		cin >> ac >> aj;
		for (int i = 0; i < ac ; ++i)
		{
			int c, d;
			cin >> c >> d;
			b.push_back(c);
			info[c] = make_pair(d, 0);
		}
		for (int i = 0; i < aj; ++i)
		{
			int c, d;
			cin >> c >> d;
			b.push_back(c);
			info[c] = make_pair(d, 1);
		}
		sort(b.begin(), b.end());
		
		long long  ans = 0, tmp[2] = {0};
		int bnow, bl, enow, el;
		
		int now = info[b[0]].second;
		cost[now].push_back(b[0]);
		tmp[now] += info[b[0]].first;
		bnow = now;
		bl = b[0];
		
		int last = info[b[0]].first;
		
		for (int i = 1; i < b.size(); ++i)
		{
			if (info[b[i]].second == now)
			{
				tmp[now] += info[b[i]].first - last;
				cost[now].push_back(b[i] - last);
			}
			else
			{
				now = info[b[i]].second;
				ans++;
			}
			
			last = info[b[i]].first;
		}
		cost[now].push_back(1440 - last);
		tmp[now] += 1440 - last;
		enow= now;
		el = 1440 - last;
		
		if (tmp[0] > 720)
		{
			sort(cost[0].begin(), cost[0].end());
			int extra = INT_MAX;
			for (int i = -1; i < 2; i += 2)
				for (int j = -1; j <2; j += 2)
				{
					int ttmp = tmp[0], sol = 0;
					if ( i == -1 && bnow != 0)
						continue;
					if ( j == -1 && enow != 0)
						continue;
					if (i == -1)
					{
						ttmp -= bl;
						sol++;
					}
					if (j == -1)
					{
						ttmp -= el;
						sol++;
					}
					if ((bnow*2 - 1)*i != (enow*2 -1)*j)
						sol++;
					
					for (int k = cost[0].size()-1 ; k >= 0 && ttmp > 720; --k)
					{
						ttmp -= cost[0][k];
						sol+=2;
					}
					if (ttmp <= 720)
						extra = min(extra, sol);
				}
			ans += extra;
		}
		else if (tmp[1] > 720)
		{
			sort(cost[1].begin(), cost[1].end());
			int extra = INT_MAX;
			for (int i = -1; i < 2; i += 2)
				for (int j = -1; j <2; j += 2)
				{
					int ttmp = tmp[1], sol = 0;
					if ( i == -1 && bnow != 1)
						continue;
					if ( j == -1 && enow != 1)
						continue;
					if (i == -1)
					{
						ttmp -= bl;
						sol++;
					}
					if (j == -1)
					{
						ttmp -= el;
						sol++;
					}
					if ((bnow*2 - 1)*i != (enow*2 -1)*j)
						sol++;
					
					for (int k = cost[1].size()-1 ; k >= 0 && ttmp > 720; --k)
					{
						ttmp -= cost[1][k];
						sol+=2;
					}
					if (ttmp <= 720)
						extra = min(extra, sol);
				}
			ans += extra;
		}
		else
		{
			if (bnow != enow)
				ans++;
		}
		
		cout << "Case #" << z << ": " << ans << endl;
	}
}
 
