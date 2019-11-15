#include <iostream>
#include <algorithm>
#include <vector>
#include <string.h>
#include <string>

using namespace std;

const string ss = "oieastbg";

int K;
string st;

bool g[36][36];
int gt[36];
int id[36], od[36];
	
long long Work()
{
	cin >> K;
	cin >> st;
	memset(g, 0, sizeof(g));

	for (int i = 0, a, b; i < st.length() - 1; i ++)
	{
		a = st[i] - 'a';  b = st[i+1] - 'a';
		g[a][b] = true;
		if (gt[a])  g[gt[a]][b] = true;
		if (gt[b])  g[a][gt[b]] = true;
		if (gt[a] && gt[b])  g[gt[a]][gt[b]] = true;
	}
	
	memset(id, 0, sizeof(id));
	memset(od, 0, sizeof(od));

	for (int i = 0; i < 34; i ++)
		for (int j = 0; j < 34; j ++)
		{
			id[i] += g[j][i];
			od[i] += g[i][j];
		}

	long long ans = 0, ts = 0;
	for (int i = 0; i < 34; i ++)
		for (int j = 0; j < 34; j ++)
			if (g[i][j])  ans ++;

	for (int i = 0; i < 34; i ++)
		if (id[i] > od[i])  ts += id[i] - od[i];

	ans += max(1LL, ts);

	return ans;
}

int main()
{
	freopen("D-small-attempt0.in", "r", stdin);
	freopen("output.txt", "w", stdout);

	cin.sync_with_stdio(false);

	memset(gt, 0, sizeof(gt));

	for (int i = 0; i < ss.length(); i ++)
		gt[ss[i]-'a'] = i + 26;

	int T;
	cin >> T;
	for (int k = 1; k <= T; k ++)
	{
		cout << "Case #" << k << ": " << Work() << endl;
	}

	return 0;
}