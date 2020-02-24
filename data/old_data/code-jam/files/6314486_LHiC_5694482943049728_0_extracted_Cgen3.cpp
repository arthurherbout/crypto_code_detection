#include <bits/stdc++.h>

#define mp make_pair
#define pb push_back


typedef long long ll;
typedef long long llong;
typedef long double ld;
typedef unsigned long long ull;

using namespace std;

template <typename T> void dprint(T begin, T end) {
    for (auto i = begin; i != end; i++) {
        cerr << (*i) << " ";
    }
    cerr << "\n";
}

random_device rd;
mt19937 rnd(rd());

int en[12000];
int mn[12000];

vector<pair<int, int> > vv;
int p[12000];

int get(int a) {
	if (a == p[a])
		return a;
	return get(p[a]);
}

int p2[12000];

int get2(int a) {
	if (a == p2[a])
		return a;
	return get2(p2[a]);
}

int run(int x, int m, int cur, int n) {
	if (cur + m - x < n - 1)
		return 0;
	for (int i = 0; i < n; ++i)
		p2[i] = p[i];
	int c2 = cur;
	for (int i = x; i < m; ++i) {
		int a = get2(vv[i].first);
		int b = get2(vv[i].second);
		if (a != b)
			p2[a] = b, ++c2;
	}
	if (c2 != n - 1)
		return 0;
	if (x == m)
		return 1;
	int ans = run(x + 1, m, cur, n);
	if (ans >= 10001)
		return ans;
	int a = get(vv[x].first);
	int b = get(vv[x].second);
	if (a != b) {
		p[a] = b;
		ans += run(x + 1, m, cur + 1, n);
		p[a] = a;
	}
	return min(ans, 10001);
}

int aans[10010][23][23];

void add(int a, int b, int c) {
	b %= mn[a];
	c %= mn[a];
	aans[a][b][c] = aans[a][c][b] = 1;
}

void print(int x) {
	cout << mn[x] << "\n";
	for (int i = 0; i < mn[x]; ++i) {
		for (int j = 0; j < mn[x]; ++j)
			cout << aans[x][i][j];
		cout << "\n";
	}
	cout.flush();
}

int check(int x) {
	vv.clear();
	int n = mn[x];
	for (int i = 0; i < n; ++i)
		if (aans[x][i][i] != 0)
			return 0;
	for (int i = 0; i < n; ++i)
		for (int j = i + 1; j < n; ++j) {
			if (aans[x][i][j] != aans[x][j][i]) {
				return 0;
			}
			if (aans[x][i][j])
				vv.push_back(make_pair(i, j));
		}
	for (int i = 0; i < n; ++i)
		p[i] = i;
	int ans = run(0, vv.size(), 0, n);
	if (ans != x) {
		return 0;
	}
	return 1;
}

int main() {
	fill(mn, mn + 11000, 1000);
	for (int n = 3; n <= 22; ++n) {
		int cc = n - 3;
		for (int i = 0; i < (1 << cc); ++i) {
			ll cur = n;
			for (int j = i; j; j = (j - 1) & i) {
				int lst = -2;
				ll now = 1;
				for (int k = 0; k < cc; ++k) {
					if ((j >> k) & 1) {
						now *= (k - lst);
						if (now > 10000)
							break;
						lst = k;
					}
				}
				now *= (cc + 1 - lst);
				cur += now;
				if (cur > 10000)
					break;
			}
			if (cur >= 3 && cur <= 10000 && !en[cur]) {
				en[cur] = 1;
				mn[cur] = n;
				for (int i = 0; i < n; ++i)
					add(cur, i, i + 1);
				for (int j = 2; j < n; ++j)
					if ((i >> (j - 2)) & 1) {
						add(cur, 0, j);
					}
			}
		}
	}
	for (int i = 1; i <= 10000; ++i) {
		for (int j = 1; j <= 10000; ++j) {
			if (en[i] && en[j] && mn[i] + mn[j] <= 23 && i * j <= 10000) {
				en[i * j] = 1;
				if (mn[i * j] > mn[i] + mn[j] - 1) {
					mn[i * j] = mn[i] + mn[j] - 1;
					for (int k = 0; k < mn[i * j]; ++k)
						for (int l = 0; l < mn[i * j]; ++l)
							aans[i * j][k][l] = 0;
					for (int k = 0; k < mn[i]; ++k)
						for (int l = 0; l < mn[i]; ++l)
							aans[i * j][k][l] = aans[i][k][l];
					for (int k = 0; k < mn[j]; ++k)
						for (int l = 0; l < mn[j]; ++l) {
							int a = k;
							int b = l;
							if (a)
								a += mn[i] - 1;
							if (b)
								b += mn[i] - 1;
							aans[i * j][a][b] = aans[j][k][l];
						}
				}
			}

		}
	}
	
	{
		ifstream in("hello");
		int cur;
		int n;
		while (in >> cur) {
			if (en[cur]) {
				in >> n;
				string s;
				for (int i = 0; i < n; ++i)
					in >> s;
			}
			else {
				en[cur] = 1;
				in >> n;
				mn[cur] = n;
				char s;
				for (int i = 0; i < n; ++i) {
					for (int j = 0; j < n; ++j) {
						in >> s;
						if (s == '1')
							aans[cur][i][j] = 1;
						else
							aans[cur][i][j] = 0;
					}
				}
			}
		}
	}
	int lft = 0;
	for (int i = 3; i <= 10000; ++i)
		if (!en[i])
			++lft;
		else {
			if (!check(i)) {
				en[i] = 0;
				++lft;
				for (int k = 0; k < 23; ++k)
					for (int l = 0; l < 23; ++l)
						aans[i][k][l] = 0;
			}
		//	print(i);
		}
	while (lft) {
		vv.clear();
		int n = rnd() % 20 + 2;
		for (int i = 0; i < n; ++i)
			for (int j = i + 1; j < n; ++j)
				vv.push_back(make_pair(i, j));
		int m = n - 1 + rnd() % 10;
		shuffle(vv.begin(), vv.end(), rnd);
		vv.resize(m);
		for (int i = 0; i < n; ++i)
			p[i] = i;
		int ans = run(0, m, 0, n);
		if (ans >= 3 && ans <= 10000 && !en[ans]) {
			mn[ans] = n;
			for (int i = 0; i < vv.size(); ++i)
				add(ans, vv[i].first, vv[i].second);
			en[ans] = 1;
			--lft;
		}
	}
	assert(check(58));
	int tt;
	cin >> tt;
	for (int i = 0; i < tt; ++i) {
		int xx;
		cin >> xx;
		printf("Case #%d: ", i + 1);
		print(xx);
	}
    return 0;
}


