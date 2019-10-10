/*
*/

#pragma comment(linker, "/STACK:16777216")
#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <iostream>
#include <string>
#include <complex>
#include <math.h>
#include <set>
#include <vector>
#include <map>
#include <queue>
#include <stdio.h>
#include <stack>
#include <algorithm>
#include <list>
#include <ctime>
#include <memory.h>
#include <assert.h>

#define y0 sdkfaslhagaklsldk
#define y1 aasdfasdfasdf
#define yn askfhwqriuperikldjk
#define j1 assdgsdgasghsf
#define tm sdfjahlfasfh
#define lr asgasgash
#define norm asdfasdgasdgsd

#define eps 1e-9
#define M_PI 3.141592653589793
#define bs 1000000007
#define bsize 350

using namespace std;

const int INF = 1e9;
const int N = 515000;

using namespace std;

int tests;
int n, l, flag, ts;
string good[N];

int main(){
	//freopen("fabro.in","r",stdin);
	//freopen("fabro.out","w",stdout);
	freopen("F:/in.txt", "r", stdin);
	freopen("F:/output.txt", "w", stdout);
	//ios_base::sync_with_stdio(0);
	//cin.tie(0);

	cin >> tests;
	for (; tests; --tests)
	{
		cin >> n >> l;
		flag = 0;
		++ts;
		cout << "Case #" << ts << ": ";

		for (int i = 1; i <= n; i++)
		{
			cin >> good[i];
		}
		string trash;
		cin >> trash;
		for (int i = 1; i <= n; i++)
		{
			if (good[i] == trash)
				flag = 1;
		}
		if (flag)
		{
			cout << "IMPOSSIBLE" << endl;
			continue;
		}
		if (l == 1)
		{
			cout << "0 ?" << endl;
			continue;
		}
		string res1, res2;
		res1 = "";
		for (int i = 1; i < l; i++)
			res1 += "?";
		for (int i = 1; i <= 50; i++)
			res2 += "10";
		res2 += "?";
		res2 += "1";
		cout << res1 << " " << res2 << endl;
	}

	cin.get(); cin.get();
	return 0;
}