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
int ts;
string st;
int cnt[100][500];
int ans;

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
		++ts;
		cin >> st;
		for (int i = 0; i <= 2; i++)
			cnt[i]['C'] = cnt[i]['J'] = 0;
		for (int i = 0; i < st.size(); i ++)
		{
			cnt[i % 2][st[i]]++;
		}
		cout << "Case #" << ts << ": ";
		ans = 0;
		int good_pairs = 0;
		good_pairs = min(cnt[0]['C'], cnt[1]['C']) + min(cnt[0]['J'], cnt[1]['J']);
		int bad_pairs = st.size() / 2;
		bad_pairs -= good_pairs;
		cout << good_pairs * 10 + bad_pairs * 5 << endl;
	}

	cin.get(); cin.get();
	return 0;
}