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

double fact[1000];
int tests;
int n;
int par[N];
vector<int> g[N];
double ways[N];
int subsize[N];
string labels;

int words_cnt;
string zv[20];
vector<int> roots;
vector<int> cur_state;
string cur_string;
int res[N];
int ts;

double C(int n, int m)
{
	if (m > n)
		return 0;
	return fact[n] / fact[n - m] / fact[m];
}

void dfs(int v)
{
	ways[v] = 1;
	subsize[v] = 1;

	for (int i = 0; i < g[v].size(); i++)
	{
		int to = g[v][i];
		dfs(to);
		subsize[v] += subsize[to];
		ways[v] *= ways[to];
		ways[v] *= C(subsize[v] - 1, subsize[to]);
	}
}

bool contains(string &st, string st1) // st1 in st
{
	for (int i = 0; i + st1.size() <= st.size(); i++)
	{
		string temp = "";
		for (int j = 0; j < st1.size(); j++)
			temp += st[j + i];
		if (temp == st1)
			return true;
	}
	return false;
}

void get_state(vector<int>&v, int ps)
{
	vector<int> res;
	for (int i = 0; i < v.size(); i++)
	{
		if (i != ps)
		{
			res.push_back(v[i]);
		}
		else
		{
			int ve = v[i];
			for (int j = 0; j < g[ve].size(); j++)
				res.push_back(g[ve][j]);
		}
	}
	v = res;
}

double eval(vector<int> &v,int id)
{
	double t_ways = 1;
	int total_size = 0;
	for (int i = 0; i < v.size(); i++)
	{
		if (i == id)
		{
			int ve = v[i];
			for (int j = 0; j < g[ve].size(); j++)
			{
				int ve2 = g[ve][j];
				t_ways = t_ways*ways[ve2];
				t_ways *= C(total_size + subsize[ve2], subsize[ve2]);
				total_size += subsize[ve2];
			}
		}
		else
		{
			t_ways = t_ways*ways[v[i]];
			t_ways *= C(total_size + subsize[v[i]], subsize[v[i]]);
			total_size += subsize[v[i]];
		}
	}
	return t_ways;
}

double sum[10000];

double rand_value()
{
	double res = 0;
	for (int i = 1; i <= 30; i++)
	{
		if (rand() % 2)
			res += (1.0 / (1ll << i));
	}
	return res;
}

int pref_size[200], suf_size[200];
double pref_ways[200], suf_ways[200];

void MK()
{
	double total_ways = 0;
	if (cur_state.size() == 0)
		return;

	pref_size[0] = 0;
	pref_ways[0] = 1;

	for (int i = 1; i <= cur_state.size(); i++)
	{
		pref_size[i] = pref_size[i - 1] + subsize[cur_state[i - 1]];
		pref_ways[i] = pref_ways[i - 1] * ways[cur_state[i - 1]] * C(pref_size[i], pref_size[i - 1]);
	}

	suf_size[cur_state.size()] = 0;
	suf_ways[cur_state.size()] = 1;

	for (int i = cur_state.size() - 1; i >= 0; --i)
	{
		suf_size[i] = suf_size[i + 1] + subsize[cur_state[i]];
		suf_ways[i] = suf_ways[i + 1] * ways[cur_state[i]] * C(suf_size[i], suf_size[i + 1]);
	}

	for (int i = 0; i < cur_state.size(); i++)
	{
		//vector<int> v2 = get_state(cur_state, i);
		double ways_here = 1;
		ways_here = pref_ways[i] * suf_ways[i + 1] * C(suf_size[i + 1] + pref_size[i], suf_size[i + 1]);
		int tsize = suf_size[i + 1] + pref_size[i];
		int v = cur_state[i];
		for (int j = 0; j < g[v].size(); j++)
		{
			int son = g[v][j];
			ways_here *= ways[son];
			ways_here *= C(tsize + subsize[son], subsize[son]);
			tsize += subsize[son];
		}
		sum[i + 1] = sum[i] + ways_here;
	}
	double step = rand_value();
	//cout << step << endl;
	int id = 0;
	for (int i = 0; i < cur_state.size(); i++)
	{
		if (sum[i + 1] >= step*sum[cur_state.size()])
		{
			id = i;
			break;
		}
	}
	cur_string += labels[cur_state[id]-1];
	get_state(cur_state, id);
	MK();
}

long long pw[N];
long long S[N], H[N];

bool contains(int id)
{
	for (int i = 0; i + zv[id].size() <= n; i++)
	{
		long long H_here = S[i + zv[id].size()] - S[i];
		long long H_need = H[id] * pw[i];
		if (H_here == H_need)
			return true;
	}
	return false;
}

int main(){
	//freopen("fabro.in","r",stdin);
	//freopen("fabro.out","w",stdout);
	freopen("F:/in.in", "r", stdin);
	freopen("F:/output.txt", "w", stdout);
	//ios_base::sync_with_stdio(0);
	//cin.tie(0);
	
	srand(31);

	pw[0] = 1;
	for (int i = 1; i <= 1000; i++)
		pw[i] = pw[i - 1] * 173;

	fact[0] = 1;
	for (int i = 1; i <= 200; i++)
	{
		fact[i] = fact[i - 1] * i;
	}
	cin >> tests;
	for (; tests; --tests)
	{
		++ts;
		cerr << ts << endl;
		cin >> n;
		for (int i = 1; i <= n; i++)
			g[i].clear();
		roots.clear();
		for (int i = 1; i <= n; i++)
		{
			cin >> par[i];
			if (par[i] != 0)
			{
				g[par[i]].push_back(i);
			}
			else
				roots.push_back(i);
		}

		for (int i = 1; i <= n; i++)
		{
			if (par[i])
				continue;
			dfs(i);
		}

		/*for (int i = 1; i <= n; i++)
		{
			cout << ways[i] << " " << subsize[i] << endl;
		}
		cout << endl;
		*/

		cin >> labels;

		cin >> words_cnt;
		for (int i = 1; i <= words_cnt; i++)
		{
			cin >> zv[i];
			H[i] = 0;
			for (int j = 1; j <= zv[i].size(); j++)
				H[i] = H[i] + zv[i][j - 1] * pw[j - 1];
			res[i] = 0;
		}
		
		for (int iter = 1; iter <= 2000; iter++)
		{
			cur_state = roots;
			cur_string = "";
			MK();
			//cout << "@" << cur_string << endl;
			for (int i = 1; i <= cur_string.size(); i++)
				S[i] = S[i - 1] + cur_string[i - 1] * pw[i - 1];

			for (int i = 1; i <= words_cnt; i++)
			{
				if (contains(i))
					++res[i];
			}
		}

		cout << "Case #" << ts << ":";
		for (int i = 1; i <= words_cnt; i++)
		{
			cout.precision(5);
			cout << " "<<1.0*res[i] / 2000;
		}
		cout << endl;
	}

	cin.get(); cin.get();
	return 0;
}