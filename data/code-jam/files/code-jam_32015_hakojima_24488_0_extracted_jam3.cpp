
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <list>

using namespace std;

#define PATH "c:\\jam3\\jam3\\"

		int C[1001][1001];
		//int CC[1001] = {0};
		int K[1001];

int main(void)
{
    ifstream fin(PATH "s.in");
    ofstream fout(PATH "s.out");

    string line;

	int i;
    int count;
    fin >> count;

	for (int t = 1; t <= count; ++t)
    {
		long long ans = 0;
		long long B[1001];
		long long n;
		long long m;
		long long X, Y,Z;

		fin >> n >> m >> X >> Y >> Z;
		long long A[101];
		for (i = 0; i < m; ++i)
			fin >> A[i];

		//printf("#%d\n", t);
		for (i = 0; i < n; ++i)
		{
			B[i] = A[i % m];
			A[i % m] = (X * A[i % m] + Y * (i + 1)) % Z;
			//printf("B:%lld\n", B[i]);
		}
		for (i = 0; i < n; ++i)
		{
			int k = 0;
			for (int j = 0; j < n; ++j)
			{
				if (B[i] < B[j])
				{
					C[i][j] = 1;
					//CC[j]++;
				}else
					C[i][j] = 0;
			}
			K[i] = 1;
		}

		for (i = 0; i < n; ++i)
		{
			for (int j = 0; j < i; ++j)
			{
				if (C[j][i])
				{
					K[i] += K[j];
					if (K[i] > 1000000007)
						K[i] %= 1000000007;
				}
			}
		}
		for (i = 0; i < n; ++i)
		{
			ans +=K[i];
			if (ans > 1000000007)
				ans %= 1000000007;
		}


        fout << "Case #" << t << ": " << ans << endl;
    }

    return 0;
}

