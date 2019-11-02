#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <map>

const long double eps = 1e-9;

using namespace std;



int a, b, p;





void Load()
{
	cin >> a >> b >> p;
}


int edg[1111][1111];
int was[1111];


void count(vector <int> &aa, int a)
{

	int i, ta;
	ta = a;
	for (i = 2; i* i <= ta; i++)  {
		if(a % i == 0) {
			if (i >= p)
				aa.push_back(i);
			while (a % i == 0) a /= i;
		}
	}
	if (a >= p) aa.push_back(a);
}


void Go(int ver)
{
	int i;
	if (was[ver] == 1) return ;
	was[ver] = 1;
	for (i = a; i <= b; i++) {
		if (edg[ver][i] == 1) Go(i);
	}
}

void Solve()
{
	int i, j;
	int k, l;
	memset(was, 0, sizeof(was));
	memset(edg, 0, sizeof(edg));
	vector <int> aa, bb;
	for (i = a; i <= b; i++) {
		aa.clear();
		count(aa, i);
		for (j = i + 1; j <= b; j++) {
			bb.clear();
			count(bb, j);
			for (k = 0; k < aa.size(); k++) {
				for (l = 0; l < bb.size(); l++) {
					if (aa[k] == bb[l] ) edg[i][j] = edg[j][i] = 1;
				}
			}
		}
	}
	int res = 0;
	for (i = a; i <= b; i++) {
		if (was[i] == 0) {
			res++;
			Go(i);
		}
	}
	cout << " " << res << "\n";
}
void Save()
{
}

int main(){
	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);
	int t, tt;
	cin >> tt;
	for (t = 1; t <= tt; t++){ 
		cerr << t << "\n";
		cout << "Case #" << t << ":";                                                 
		Load();
		Solve();
		Save();
	}
	return 0;
}