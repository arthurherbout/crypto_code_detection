#include <cmath>
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>

using namespace std;

#define foreach(e,x) for(__typeof((x).begin()) e=(x).begin(); e!=(x).end(); ++e)

const int N = 500000 + 10;
const long double EPS = 1e-12;

int n;
int start;
long double f;
char a[N];
int s[N];
long double p[N], q[N];
int u[N], v[N];
int flag;
int bit[N], tag[N];

void update(int k)
{
	if (k < 0) return;
	if (start < 0 || start > k) {
		start = k;
	}
}

int cmp(const pair<int, int> &a, const pair<int, int> &b)
{
	int ai = a.first, bi = b.first;
	if (v[ai] != v[bi]) {
		return v[ai] > v[bi];
	}
	if (u[ai] != u[bi]) {
		return u[ai] < u[bi];
	}
	return a.second < b.second;
}

void insert(int v, int id)
{
	//cout << "insert " << v << ' ' << id << endl;
	for(v ++; v < N; v += v & -v) {
		if (tag[v] != flag) {
			tag[v] = flag;
			bit[v] = N;
		}
		bit[v] = min(bit[v], id);
	}
}

int query(int v)
{
	int ret = N;
	//cout << "query " << v << endl;
	for(v ++; v; v -= v & -v) {
		if (tag[v] != flag) {
			tag[v] = flag;
			bit[v] = N;
		}
		ret = min(ret, bit[v]);
	}
	if (ret == N) return -1;
	return ret;
}

void solve(int l, int r)
{
	if (l >= r) {
		return;
	}
	int mid = (l + r) >> 1;
	solve(l, mid);
	solve(mid + 1, r);
	vector<pair<int, int> > vec;
	for(int i = l; i <= mid; ++ i) {
		vec.push_back(make_pair(i, 0));
	}
	for(int i = mid + 1; i <= r; ++ i) {
		vec.push_back(make_pair(i, 1));
	}
	sort(vec.begin(), vec.end(), cmp);
	flag ++;
	//cout << "IN" << endl;
	for(int i = 0; i < vec.size(); ++ i) {
		int id = vec[i].first;
		//cout << "id " << id << endl;
		if (vec[i].second == 0) {
			insert(u[id], id);
		} else {
			update(query(u[id]));
		}
	}
	//cout << l << ' ' << r << ' ' << mid << ' ' << start << endl;
	//cout << "OUT" << endl;
}

long double c[N];

void getIt(long double *a, int *b)
{
	for(int i = 0; i <= n; ++ i) {
		c[i] = a[i];
	}
	sort(c, c + n + 1);
	for(int i = 0; i <= n; ++ i) {
		b[i] = lower_bound(c, c + n + 1, a[i] - EPS) - c;
	}
}

int checkOk(long double lim)
{
	start = -1;
	for(int i = 0; i <= n; ++ i) {
		p[i] = s[i] - i * f + i * lim;
		q[i] = s[i] - i * f - i * lim;
	}
	getIt(p, u);
	getIt(q, v);
	for(int i = 0; i <= n; ++ i) {
		//cout << u[i] << ' ' << v[i] << endl;
	}
	solve(0, n);
}

void solve()
{
	cin >> n >> f;
	scanf("%s", a + 1);
	s[0] = 0;
	for(int i = 1; i <= n; ++ i) {
		s[i] = s[i - 1] + (a[i] == '1');
	}
	//checkOk(1 + 1e-9);
	//cout << start << endl;
	//return;

	long double l = 0, r = 1 + 1e-9, mid;
	for(int i = 0; i < 50; ++ i) {
		mid = (l + r) * 0.5;
		//cout << mid << endl;
		checkOk(mid);
		if (start < 0) {
			l = mid;
		} else {
			r = mid;
		}
	}
	checkOk(r + EPS);
	cout << start << endl;
	
}

int main()
{
	//freopen("A-small-attempt0.in", "r", stdin); freopen("A-small-attempt0.out", "w", stdout);
	//freopen("A-small-attempt1.in", "r", stdin); freopen("A-small-attempt1.out", "w", stdout);
	//freopen("A-small-attempt2.in", "r", stdin); freopen("A-small-attempt2.out", "w", stdout);
	//freopen("A-large.in", "r", stdin); freopen("A-large.out", "w", stdout);
	int test_case;
	cin >> test_case;
	for(int i = 0; i < test_case; ++ i) {
		printf("Case #%d: ", i + 1);
		//cerr << "now " << i << endl;
		solve();
	}
	return 0;
}
