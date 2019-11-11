#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

const long double eps = 1e-9;




int n;
map <string, int> clrs;
int nc;


vector <int> st[333], en[333];




void Load()
{
	cin >> n;
	int i, j, k, t;
	string s;
	clrs.clear();
	for (i = 1; i <= n; i++) {
		cin >> s;
		if (clrs.find(s) == clrs.end()) {
			j = clrs.size();
			st[j].clear();
			en[j].clear();
			clrs[s] = j;	
		}
		j = clrs[s];
		cin >> k >> t;
		st[j].push_back(k);
		en[j].push_back(t);
	}
	nc = clrs.size();
}

vector <pair <int, int> > tmp;
int res;
int was[11111];


void Go(int ii, int jj, int kk)
{
	int i, j, k;
	tmp.clear();
	memset(was, 0, sizeof(was));
	if (ii < nc) for (i = 0; i < st[ii].size(); i++) tmp.push_back(make_pair(st[ii][i], en[ii][i]));
	if (jj < nc) for (i = 0; i < st[jj].size(); i++) tmp.push_back(make_pair(st[jj][i], en[jj][i]));
	if (kk < nc) for (i = 0; i < st[kk].size(); i++) tmp.push_back(make_pair(st[kk][i], en[kk][i]));
	vector <int> ww;
	ww.resize(tmp.size());
	int tres = 0;
	std::sort(tmp.begin(), tmp.end());
	for (i = 1; i <= 10000; i++) {
		if (was[i] == 1) continue;
		k = -1;
		for (j = 0; j < tmp.size(); j++) {
			if (ww[j]) continue;
			if (tmp[j].first <= i && (tmp[j].second >= i)) {
				if (k == -1 || tmp[k].second < tmp[j].second) k = j;
			}
			
		}
		if (k == -1) return;
		ww[k] = 1;
		tres++;
		for (j = i; j <= tmp[k].second; j++) was[j] = 1;
	}
	if (tres < res || res < 0) res = tres;	

}


void Solve()
{
	res = -1;
	int i, j, k;
	if (nc  <= 3) {
		Go(0,1,2);
		
	}   else
	for (i = 0; i < nc; i++) {
		for (j = i + 1; j < nc; j++) {
			for (k = j + 1; k < nc; k++) {
				Go(i,j,k);
			}
		}
	}
	if (res == -1) cout << "IMPOSSIBLE\n";
	else cout << res << "\n";
}


int main()
{
	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);
	int tt, t;
	cin >> tt;
	for (t = 1; t <= tt; t++) {
		cout << "Case #" << t << ": ";
		Load();
		Solve();
	}
	return 0;
}