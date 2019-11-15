#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
using namespace std;

#define mp(a,b) make_pair(a,b)

struct Arc {
	int f, s, prev, a, b;
} arr[4005];


int beg[2005], kol;
int d[1005];
int dd[1005];
int md[1005];
int id[505];

void AddArc(int f, int s, int a, int b) {
	arr[kol].f = f;
	arr[kol].s = s;
	arr[kol].a = a;
	arr[kol].b = b;
	arr[kol].prev = beg[f];
	beg[f] = kol++;

	arr[kol].f = s;
	arr[kol].s = f;
	arr[kol].a = a;
	arr[kol].b = b;
	arr[kol].prev = beg[s];
	beg[s] = kol++;
}

void Update(int v) {
	set<pair<int, int>> S;
	md[v] = d[v];
	S.insert(mp(md[v], v));
	while (!S.empty()) {
		pair<int, int> par = *S.begin();
		S.erase(S.begin());
		if (md[par.second] != par.first) {
			continue;
		}
		int v = par.second;
		for (int i = beg[v]; i != -1; i = arr[i].prev) {
			if (!(i & 1)) {
				continue;
			}
			if (md[arr[i].s] < md[v] - arr[i].b) {
				md[arr[i].s] =  md[v] - arr[i].b;
				S.insert(mp(md[arr[i].s], arr[i].s));
			}
		}
	}

}

bool Check(int v) {
	memset(dd, 0x3f, sizeof dd);
	set<pair<int, int>> S;
	dd[0] = 0;
	S.insert(mp(0, 0));
	while (!S.empty()) {
		pair<int, int> par = *S.begin();
		S.erase(S.begin());
		if (dd[par.second] != par.first) {
			continue;
		}
		int v = par.second;
		for (int i = beg[v]; i != -1; i = arr[i].prev) {
			if (i & 1) {
				continue;
			}
			if (dd[arr[i].s] > max(md[arr[i].s], dd[v] + arr[i].b)) {
				dd[arr[i].s] = max(md[arr[i].s], dd[v] + arr[i].b);
				if (dd[arr[i].s] > dd[v] + arr[i].b) {
					return false;
				}
				S.insert(mp(dd[arr[i].s], arr[i].s));
			}
		}
	}
	int req = dd[1];

	memset(dd, 0x3f, sizeof dd);
	dd[v] = d[v];
	S.insert(mp(dd[v], v));
	while (!S.empty()) {
		pair<int, int> par = *S.begin();
		S.erase(S.begin());
		if (dd[par.second] != par.first) {
			continue;
		}
		int v = par.second;
		for (int i = beg[v]; i != -1; i = arr[i].prev) {
			if (i & 1) {
				continue;
			}
			if (dd[arr[i].s] > max(md[arr[i].s], dd[v] + arr[i].a)) {
				dd[arr[i].s] = max(md[arr[i].s], dd[v] + arr[i].a);
				S.insert(mp(dd[arr[i].s], arr[i].s));
			}
		}
	}
	if (req < dd[1]) {
		return false;
	} else {
		return true;
	}
}

void Solve() {
	kol = 0;
	memset(beg, -1, sizeof beg);
	int n, m, p;
	cin >> n >> m >> p;
	for (int i = 0; i < m; ++i) {
		int f, s, a, b;
		cin >> f >> s >> a >> b;
		AddArc(f - 1, s - 1, a, b);
	}
	for (int i = 0; i < p; ++i) {
		cin >> id[i];
		--id[i];
		id[i] *= 2;
	//	arr[id[i]].b = arr[id[i]].a;
	}
	memset(d, 0x3f, sizeof d);
	memset(md, 0, sizeof md);
	d[0] = 0;
	int pos = -1;
	for (int i = 0; i < p; ++i) {
		arr[id[i]].b = arr[id[i]].a;
		if (d[arr[id[i]].s] != 0x3f3f3f3f) {
			pos = id[i] / 2 + 1;
			break;
		}
		d[arr[id[i]].s] = d[arr[id[i]].f] + arr[id[i]].a;
		Update(arr[id[i]].s);
		if (!Check(arr[id[i]].s)) {
			pos = id[i] / 2 + 1;
			break;
		}
	}
	if (pos != -1) {
		printf("%d\n", pos);
	} else {
		puts("Looks Good To Me");
	}
}


int main() {
	freopen("c.in", "r", stdin);
	freopen("c.out", "w", stdout);
	int T;
	scanf("%d", &T);
	for (int I = 0; I < T; ++I) {
		printf("Case #%d: ", I + 1);
		Solve();
	}
	return 0;
}