/*
 * 2011-05-21  Martin  <Martin@Martin-desktop>

 * 
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

using namespace std;

#define MaxiN 10005
#define MaxiM 105

int N, M;
string Word[MaxiN], Alpha[MaxiM];

void Solve()
{
	cin >> N >> M;
	for (int i = 1; i <= N; ++ i)
		cin >> Word[i];
	for (int i = 1; i <= M; ++ i)
		cin >> Alpha[i];
	for (int i = 1; i <= M; ++ i)
	{
		int Ans = - 100;
		string Best = "";
		for (int j = 1; j <= N; ++ j)
		{
			int Now = 0;
			set <int> Set;
			Set.clear();
			for (int g = 1; g <= N; ++ g)
				if (Word[g].length() == Word[j].length() && g != j)
					Set.insert(g);
			vector <int> Need;
			Need.clear();
			for (int k = 0; k < (int) Alpha[i].length(); ++ k)
			{
				bool Have = 0;
				bool Ought = 1;
				for (int t = 0; t < (int) Word[j].length(); ++ t)
					if (Word[j][t] == Alpha[i][k])
						Ought = 0;
				for (set <int> :: iterator it = Set.begin(); it != Set.end(); ++ it)
					for (int t = 0; t < (int) Word[j].length(); ++ t)
						if ((Word[j][t] == Alpha[i][k]) ^ (Word[*it][t] == Alpha[i][k]))
						{
							Have |= Ought;
							Need.push_back(*it);
							break;
						}
				while (!Need.empty())
					Set.erase(Need.back()), Need.pop_back();
				Now += Have;
			}
			if (Now > Ans)
			{
				Ans = Now;
				Best = Word[j];
			}
		}
		cout << Best << ((i == M) ? '\n' : ' ');
	}
}

int main()
{
	int TestCase;
	scanf("%d", &TestCase);
	for (int i = 1; i <= TestCase; ++ i)
	{
		printf("Case #%d: ", i);
		Solve();
	}
	return 0;
}
