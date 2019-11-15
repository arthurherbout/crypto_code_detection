#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>

using namespace std;

#define MMD 1000000007

bool vis[101];

bool solve(int x, string s[], int n, bool cx[][30])
{
	string outp = s[x];
	bool flag = true;

	bool chux[30];
	memset(chux, 0, sizeof(chux));
	for (int i = 0 ; i < outp.length() ; i++)
		chux[outp[i] - 'a'] = true;

	while (flag)
	{
		flag = false;
		for (int i = 0 ; i < n ; i++)
		{
			if (!vis[i])
			{
				if (s[i][0] == outp[outp.length() - 1])
				{
					for (int j = 0 ; j < 26 ; j++)
					{
						if (cx[i][j] && chux[j] && s[i][0] - 'a' != j)
						{
							return false;
						}
					}
					flag = true;
					vis[i] = true;
					outp = outp + s[i];
					for (int j = 0 ; j < s[i].length() ; j++)
						chux[s[i][j] - 'a'] = true;
				}
				else if (s[i][s[i].length() - 1] == outp[0])
				{
					for (int j = 0 ; j < 26 ; j++)
					{
						if (cx[i][j] && chux[j] && outp[0] - 'a' != j)
						{
							return false;
						}
					}
					flag = true;
					vis[i] = true;
					outp = s[i] + outp;
					for (int j = 0 ; j < s[i].length() ; j++)
						chux[s[i][j] - 'a'] = true;
				}
				else {
					for (int j = 0 ; j < 26 ; j++)
					{
						if (cx[i][j] && chux[j])
						{
							return false;
						}
					}
				}
			}
		}
	}

	return true;
}

void jiecheng(long long &x, long long now)
{
	for (long long i = 2 ; i <= now ; i++)
		x = (x * i) % MMD;
}

int main()
{
	freopen("B-small-attempt0.in", "r", stdin);
	freopen("B-small-attempt0.out", "w", stdout);
	int T;
	cin >> T;
	for (int tt = 1 ; tt <= T ; tt++)
	{
		int n;
		cin >> n;
		string s[101];
	
		long long all_s[30];
		long long no_s[30];
		memset(all_s, 0, sizeof(all_s));
		memset(no_s, 0, sizeof(no_s));
		memset(vis, 0, sizeof(vis));
		bool cx[101][30];
		memset(cx, 0, sizeof(cx));
		bool flag = true;

		for (int i = 0 ; i < n ; i++)
		{
			cin >> s[i];
			bool alf[30];
			memset(alf, 0, sizeof(alf));
			int tot_s = 0;
			for (int j = 0 ; j < s[i].length() ; j++)
			{
				cx[i][s[i][j] - 'a'] = true;
				if (alf[s[i][j] - 'a'] == false)
				{
					tot_s ++;
					alf[s[i][j] - 'a'] = true;
				}
			}

			if (tot_s == 1)
			{
				all_s[s[i][0] - 'a'] ++;
				vis[i] = true;
			}
			else {
				for (int j = 0 ; j < 26 ; j++)
				{
					if (alf[j])
					{
						no_s[j] ++;
					}
				}
			}

			if (s[i].length() > 2)
			{
				for (int j = 0 ; j < s[i].length() - 2 ; j++)
					if (s[i][j] != s[i][j + 1])
					{
						for (int k = j + 2 ; k < s[i].length() ; k++)
						{
							if (s[i][j] == s[i][k])
							{
								flag = false;
							}
						}
					}
			}
		}

		long long ans = 1;
		for (int i = 0 ; i < 26 ; i++)
		{
			if (no_s[i] > 2)
			{
				flag = false;
				break;
			}
		}


		long long tot_g = 0;

		if (flag)
		{
			for (int i = 0 ; i < n ; i++)
				if (!vis[i])
				{
					vis[i] = true;
					tot_g ++;
					if (flag == false) break;
					flag = solve(i, s, n, cx);
				}
		}

		if (flag)
		{
			for (int i = 0 ; i < 26 ; i++)
			{
				if (all_s[i] && no_s[i])
					jiecheng(ans, all_s[i]);
				else if (all_s[i] && !no_s[i])
				{
					jiecheng(ans, all_s[i]);
					tot_g ++;
				}
			}
			jiecheng(ans, tot_g);
			cout << "Case #" << tt << ": " << ans << endl;
		}
		else
			cout << "Case #" << tt << ": 0" << endl;
	}
}