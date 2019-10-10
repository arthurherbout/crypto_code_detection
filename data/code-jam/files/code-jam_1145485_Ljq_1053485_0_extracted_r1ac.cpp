#include <stdio.h>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

struct sCard
{
	int c, s, t;
};


int ans, sum, vc;
int n, m;
vector <sCard> hand, desk;

bool cmp(const sCard &a, const sCard &b)
{
	return (a.s > b.s)||(a.s == b.s && a.c > b.c);
}

int dfs(int lt, int ens, int dsb)
{
	if (lt == 0 || hand.empty())	
	{
		ans = max(ans, ens);
		vc ++;
		return 0;
	}
	vector <sCard> sh;
	
	sort(hand.begin(), hand.end(), cmp);

	sh = hand;
	int lm = hand.size(), mt = -1;
	for (int i = 0; i < hand.size(); i ++)
		mt = max(mt, hand[i].t);

	for (int i = 0; i < hand.size(); i ++)
		if (hand[i].t == mt)
		{
			if (ans == sum || vc > 3000000)  break;

			sCard sc = hand[i];

			swap(hand[i], hand[hand.size()-1]);
			hand.pop_back();

			for (int i = 0; i < sc.c && dsb + i < desk.size(); i ++)
				hand.push_back(desk[dsb+i]);
			
			
			dfs(lt + mt - 1, ens + sc.s, dsb + sc.c);

			hand = sh;
		}

	return 0;
}

int work()
{
	scanf("%d", &n);
	sCard sc;
	hand.clear();  desk.clear();   sum = 0;
	for (int i = 0; i < n; i ++)
	{
		scanf("%d%d%d", &sc.c, &sc.s, &sc.t);
		sum += sc.s;
		hand.push_back(sc);
	}
	scanf("%d", &m);

//	printf("%d\n", n + m);

	for (int i = 0; i < m; i ++)
	{
		scanf("%d%d%d", &sc.c, &sc.s, &sc.t);
		desk.push_back(sc);
	}
	ans = 0;   vc = 0;

	dfs(1, 0, 0);

	printf("%d\n", ans);

	return 0;
}


int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int T;
	scanf("%d", &T);
	for (int k = 1; k <= T; k ++)
	{
		printf("Case #%d: ", k);
		work();
	}
	fclose(stdin);
	fclose(stdout);
	return 0;
}